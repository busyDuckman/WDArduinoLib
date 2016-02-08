#include "GNUPlot.h"

//--------------------------------------------------------------------------------------------------------------------------------------
// creation of line or point graph
void GNUPlotXYGraphBase::createGraph()
{
	//setup vars
	setVaribleByString_nullSafe(F("xlabel"), xAxisName);
	setVaribleByString_nullSafe(F("ylabel"), yAxisName);
	setVaribleByString_nullSafe(F("title"), graphTitle);
	setVaribleFlagByBoolean(F("grid"), showGrid);
	printLogAxisCommand(F("y"), yAxisLogScale);
	printLogAxisCommand(F("x"), xAxisLogScale);
	

	//do the plot
	outStream->print(F("plot "));
	for(int i=0; i<numYSeries; i++)
	{
		//eg: using 2:1 with lines
		outStream->print(F("print_filename using "));

		if(xAxisSpecified)
		{
			outStream->print(F("1:"));
		}
		outStream->print(i+ (xAxisSpecified?2:1));

		//style
		switch(getGraphStyle())
		{
		case GraphStyle::LinePlot:
				outStream->print(F(" with linespoints"));
				break;
		case GraphStyle::PointPlot:
				outStream->print(F(" with points"));
				break;
		}

		//title for this series
		printTitleInPlotCommand(i);

		//style of series
		printLineApperance(seriesApperance[i]);

		//comma for next data series
		if(i<(numYSeries-1))
		{
			outStream->print(F(", "));
		}
	}
	outStream->println();

	//We now have a plot that is hard to read because the autorand places data right on the border.
	//So we use some variables created by the initial plot command to calculate a new range.
	outStream->println(F("set yrange [GPVAL_Y_MIN-(GPVAL_Y_MAX-GPVAL_Y_MIN)*0.05:GPVAL_Y_MAX+(GPVAL_Y_MAX-GPVAL_Y_MIN)*0.05]"));
	outStream->println(F("set xrange [GPVAL_X_MIN-(GPVAL_X_MAX-GPVAL_X_MIN)*0.05:GPVAL_X_MAX+(GPVAL_X_MAX-GPVAL_X_MIN)*0.05]"));
	outStream->println(F("replot"));
}

//--------------------------------------------------------------------------------------------------------------------------------------
// creation of bar graph
void GNUPlotBarGraph::createGraph()
{
	//setup vars
	setVaribleByString_nullSafe(F("xlabel"), xAxisName);
	setVaribleByString_nullSafe(F("ylabel"), yAxisName);
	setVaribleByString_nullSafe(F("title"), graphTitle);
	setVaribleFlagByBoolean(F("grid"), showGrid);
	printLogAxisCommand(F("y"), yAxisLogScale);
	outStream->println(F("set style fill solid"));
	outStream->println(F("set auto x"));
	outStream->println(F("set style data histogram"));
	outStream->println(F("set style histogram cluster gap 1"));
	outStream->println(F("set style fill solid border -1"));
	outStream->println(F("set boxwidth 0.9"));
	outStream->println(F("set xtic scale 0"));

	if(xAxisSpecified)
	{
		outStream->print(F("plot print_filename using 2:xtic(1) "));
	}
	else
	{
		outStream->print(F("plot print_filename using 1 "));
	}

	for(int i=0; i<numYSeries; i++)
	{
		int currentCol =  xAxisSpecified ? (i+2) : (i+1);
		
		if(i != 0)
		{
			outStream->print(F(" '' u "));
			outStream->print(currentCol);
		}
	
		//title for this series
		printTitleInPlotCommand(i);

		//style of series
		printLineApperance(seriesApperance[i]);

		//comma for next data series
		if(i<(numYSeries-1))
		{
			outStream->print(F(", "));
		}
	}
	outStream->println();

	//we are corecting for the bar plot not ranging to 0 sometiomes.
	outStream->println(F("barMin = (GPVAL_Y_MIN>0)?0:GPVAL_Y_MIN"));
	outStream->println(F("barMax = (GPVAL_Y_MAX<0)?0:GPVAL_Y_MAX"));
	outStream->println(F("barMax = barMax+(barMax-barMin)*0.05"));
	outStream->println(F("set yrange [barMin:barMax]"));
	outStream->println(F("replot"));
}

//--------------------------------------------------------------------------------------------------------------------------------------
void GNUPlotBase::printTitleInPlotCommand(int seriesNum)
{
	outStream->print(F(" title \""));
	if(seriesNames[seriesNum] != NULL)
	{
		outStream->print(seriesNames[seriesNum]);
	}
	else
	{
		outStream->print(F("Series #"));
		outStream->print(seriesNum);
	}
	outStream->print(F("\" "));
}

//--------------------------------------------------------------------------------------------------------------------------------------
void GNUPlotBase::printLineApperance(LineApperance apperance)
{
	//determine applicable plot attributes.
	bool plotHasLines=true;
	bool plotHasPoints=true;
	bool plotHasFill=true;

	switch(getGraphStyle())
	{
	case GraphStyle::PointPlot:
		plotHasLines = false;
		plotHasFill = false;
		break;
	case GraphStyle::LinePlot:
		plotHasFill = false;
		break;
	case GraphStyle::BarGraph:
		plotHasPoints = false;
		plotHasLines = false;
		break;
	}


	//line color
	outStream->print(F(" lt rgb \""));
	apperance.printRGBAsHTMLCode(outStream);
	outStream->print(F("\""));

	if(plotHasLines)
	{
		//line width
		outStream->print(F(" linewidth "));
		outStream->print(apperance.getLineWidth());

		//LineStyle
		LineStyles style = apperance.getLineStyle();
		if(style != LineStyles::SolidLine)
		{
			outStream->print(F(" dashtype '"));
			switch(apperance.getLineStyle())
			{
				case LineStyles::DashDotLine:
					outStream->print(F("-.'"));
					break;
				case LineStyles::DashedLine:
					outStream->print(F("-'"));
					break;
				case LineStyles::DotedLine:
					outStream->print(F(".'"));
					break;
			}
		}
	}

	if(plotHasPoints)
	{
		//point type
		outStream->print(F(" pointtype "));
		switch(apperance.getMarkerStyle())
		{
			case NoMarker:
				outStream->print(0);
				break;
			case CircleMarker:
				outStream->print(6);
				break;
			case SquareMarker:
				outStream->print(4);
				break;
			case DiamondMarker:
				outStream->print(12);
				break;
			case  CrossMarker:
				outStream->print(1);
				break;
			case SolidCircleMarker:
				outStream->print(7);
				break;
			case SolidSquareMarker:
				outStream->print(5);
				break;
			case SolidDiamondMarker:
				outStream->print(13);
				break;
		}
	}
	
}

//--------------------------------------------------------------------------------------------------------------------------------------
void GNUPlotBase::savePlotToImageFile(const char* title)
{
	//set terminal pngcairo
	outStream->println(F("set terminal pngcairo size 800,600 dashed"));
	
	//set output "my cool graph.png"
	outStream->print(F("set output \""));
	if(title != NULL)
	{
		outStream->print(title);
	}
	else
	{
		outStream->print(F("untitled_graph_output"));
	}
	outStream->println(F(".png\""));

	outStream->println(F("replot"));
	outStream->println(F("unset output"));
	outStream->println(F("unset terminal"));

}

//--------------------------------------------------------------------------------------------------------------------------------------
void GNUPlotBase::makeReadyForPlotData()
{
	SerialGraph::makeReadyForPlotData();

	char *fileName = mallocBasicFileNameFromGraphTitle();
	
	outStream->print(F("print_filename = \""));
	outStream->print(fileName);
	outStream->println(F("_PlotData_.dat\""));
	outStream->println(F("set print print_filename"));

	free(fileName);	
}

//--------------------------------------------------------------------------------------------------------------------------------------
void GNUPlotBase::_printDataValue(const char *value)
{
	//this method uses escaped strings to create an escaped string...
	//the output should look like:  " \"", "my value", "\" "

	//    " \"", "
	outStream->print(F("\"\\\"\", \""));
	char *buffer = mallocSanitizedCopy(value,  StringValidationRules::NoQuotes | 
											StringValidationRules::ComPortSafe | 
											StringValidationRules::NoNewLines, 
											maxStringSize);
	outStream->print(buffer);
	//    ", "\" ",
	outStream->print(F("\", \"\\\" \" "));

	free(buffer);
}

//--------------------------------------------------------------------------------------------------------------------------------------
void GNUPlotBase::printLogAxisCommand(const __FlashStringHelper* axisName, byte logScale)
{
	if(logScale > 1)
	{
		outStream->print(F("set logscale "));
		outStream->print(axisName);
		outStream->print(F(" "));
		outStream->println(logScale);
	}
	else
	{
		outStream->print(F("unset logscale "));
		outStream->println(axisName);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------
//setup plot
void GNUPlotBase::newGraph()
{
	printComment(F("-- Arduino to GNUPlot interface                --"));
	printComment(F("-- Author: duckman  Ver: 1.0  Date: 11/12/2015 --"));
	printComment(F("-- CC-BY (www.busyducks.com)                   --"));
	printComment(F("-- Requires GNUPlot v5.0 or later              --"));
		
	outStream->println(F("reset"));
	init();
}

//--------------------------------------------------------------------------------------------------------------------------------------
//render plot, save output
void GNUPlotBase::finishGraph()
{
	//terminate data input
	outStream->println(F("set print"));

	//plot
	createGraph();

	//prepare save file
	if(this->saveFile)
	{
		char *fileName = mallocBasicFileNameFromGraphTitle();
		savePlotToImageFile(fileName);
		free(fileName);	
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------
// the callee must free..., does not include any extension.
char* GNUPlotBase::mallocBasicFileNameFromGraphTitle()
{
	if(graphTitle != NULL)
	{
		return mallocSanitizedCopy(graphTitle, 
							       StringValidationRules::FileNameNoExt |  StringValidationRules::ComPortSafe   | 
							       StringValidationRules::NoNewLines    |  StringValidationRules::NoQuotes, 
								   maxStringSize);
	}
	else
	{
		return mallocCopyOf(F("UNTITLED_GRAPH"));
	}
}