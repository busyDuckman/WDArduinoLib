// 
// 
// 

#include "SerialGraph.h"

SerialGraph::SerialGraph(Print *_outStream)
{
	outStream = _outStream;
	seriesNames = (char ** )malloc(maxSeries*sizeof(char *));
	seriesApperance = (LineApperance *)malloc(maxSeries*sizeof(LineApperance));

	init();
}

//--------------------------------------------------------------------------------------------------------------------------------------
void SerialGraph::init()
{
	//free series names
	for(int i =0; i<maxSeries; i++)
	{
		if(seriesNames[i] != NULL)
		{
			free(seriesNames[i]);
		}
	}
	//NULL series names
	memset(seriesNames, 0, maxSeries*sizeof(char *));

	//setup defaul line styles
	long lineApperanceList[12] = {Spectral1, Spectral2, Spectral3, Spectral4, Spectral5, Spectral6, Spectral7, Spectral8, Spectral9, Spectral10, Spectral11, Spectral12};
	for(int i =0; i<maxSeries; i++)
	{
		seriesApperance[i].init();
		seriesApperance[i].setRGB(lineApperanceList[i%12]);
	}

	//reset all graph data to default
	xAxisSpecified=false;
	numYSeries=0;

	showGrid = false;
	graphTitle = NULL;
	xAxisName = NULL;
	yAxisName = NULL;

	xAxisLogScale = 0;
	yAxisLogScale = 0;
}


//--------------------------------------------------------------------------------------------------------------------------------------
SerialGraph::~SerialGraph()
{
	//free series names
	for(int i =0; i<maxSeries; i++)
	{
		if(seriesNames[i] != NULL)
		{
			free(seriesNames[i]);
		}
	}

	//clean up other memory
	free(seriesNames);
	free(seriesApperance);
}

//--------------------------------------------------------------------------------------------------------------------------------------
void SerialGraph::ensureReadyToRecivePlotData()
{
	//we have not set up an series yet
	if(numYSeries == 0)
	{
		makeReadyForPlotData();
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------
void SerialGraph::makeReadyForPlotData()
{
		
}