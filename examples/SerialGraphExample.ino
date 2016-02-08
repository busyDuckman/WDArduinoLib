#include <GNUPlot.h>
#include <Sanity.h>


void setup()
{
	Serial.begin(CommonBaudRates::BaudRate115200);
	
	GNUPlotLineGraph gnuLinePlotter(&Serial);
	gnuLinePlotter.printComment(F("SerialGraph Examples")); 
	
	PlotDampenedSineWaves(gnuLinePlotter);
	PlotLogarithmicSpiral(gnuLinePlotter);

	GNUPlotPointGraph gnuPointPlotter(&Serial);
	PlotPointNoise(gnuPointPlotter);

	GNUPlotBarGraph gnuBarPlotter(&Serial);
	PlotSampleBarGraph(gnuBarPlotter);	
}

void loop() { }

/** 
 * @example GNUPlot.h
 * Shows how to construct a line graph.
 * @verbatim
 * Serial.begin([your baude rate here]);
 * GNUPlotLineGraph gnuLinePlotter(&Serial);
 * PlotDampenedSineWaves(gnuLinePlotter);
 * @endverbatim
 */
void PlotDampenedSineWaves(SerialGraph& plot)
{
	
	//setup graph
	plot.newGraph();
	plot.enableSaveImageFile(true);
	plot.setTitle(F("Dampened Sine Wave"));
	plot.setXAxisName(F("Time (Sec)"));
	plot.setYAxisName(F("Voltage (V)"));

	//setup series
	plot.setSeriesName(0, F("Slow decay"));
	plot.getLineApperance(0)->setLineStyle(LineStyles::SolidLine);
	plot.getLineApperance(0)->setLineWidth(4);
	plot.getLineApperance(0)->setRGB(GraphColorsSchemeBlueScale::Blue1);


	plot.setSeriesName(1, F("Medium decay"));
	plot.getLineApperance(1)->setLineStyle(LineStyles::DashedLine);
	plot.getLineApperance(1)->setLineWidth(3);
	plot.getLineApperance(1)->setRGB(GraphColorsSchemeBlueScale::Blue2);

	plot.setSeriesName(2, F("Quick decay"));
	plot.getLineApperance(2)->setLineStyle(LineStyles::DotedLine);
	plot.getLineApperance(2)->setLineWidth(2);
	plot.getLineApperance(2)->setRGB(GraphColorsSchemeBlueScale::Blue3);

	//plot graph
	for(int i=0; i<=720; i+=3)
	{
		float c = cos(DEG_TO_RAD*(float)i); 
		float y1 = Dampen(i, c, 1, 1.0/360.0); 
		float y2 = Dampen(i, c, 1, 1.0/240.0); 
		float y3 = Dampen(i, c, 1, 1.0/180.0); 

		plot.plotDatumXYn(i, y1, y2, y3);
	}
	
	plot.finishGraph();
}

/** 
 * @example GNUPlot.h
 * Shows how to construct a line graph that is not 1 for 1.
 * @verbatim
 * Serial.begin([your baude rate here]);
 * GNUPlotLineGraph gnuLinePlotter(&Serial);
 * PlotLogarithmicSpiral(gnuLinePlotter);
 * @endverbatim
 */
void PlotLogarithmicSpiral(SerialGraph& plot)
{
	//setup graph
	plot.newGraph();
	plot.enableSaveImageFile(true);
	plot.setTitle(F("Logarithmic Spiral"));

	//setup series
	plot.setSeriesName(0, F("Spiral"));
	plot.getLineApperance(0)->setLineWidth(2);
	plot.getLineApperance(0)->setMarkerStyle(LineMarkers::SolidCircleMarker);
	plot.getLineApperance(0)->setRGB(GraphColorsSchemeSpectral::Spectral7);
		
	//plot graph
	float a = 1;
	float b = 0.1759;
	for(int i=0; i<(int)(10.0*20.0*M_PI); i+=5)
	{
		float t = ((float)i) * 0.1;
		float rt = a*pow(M_E, b* t);
		float x = rt*cos(t);
		float y = rt*sin(t);

		plot.plotDatumXY(x, y);
	}

	plot.finishGraph();
}

/** 
 * @example GNUPlot.h
 * Shows how to plot a point graph.
 * @verbatim
 * Serial.begin([your baude rate here]);
 * GNUPlotPointGraph gnuPointPlotter(&Serial);
 * PlotPointNoise(gnuPointPlotter);
 * @endverbatim
 */
void PlotPointNoise(SerialGraph& plot)
{
	//setup graph
	plot.newGraph();
	plot.enableSaveImageFile(true);
	plot.setTitle(F("Sine Waves with Noise"));
	plot.setXAxisName(F("Time (Sec)"));
	plot.setYAxisName(F("Voltage (V)"));
	plot.setShowGrid(true);

	//setup series
	plot.setSeriesName(0, F("Pin 1"));
	plot.getLineApperance(0)->setMarkerStyle(LineMarkers::SquareMarker);
	plot.getLineApperance(0)->setRGB(0xff0000);

	plot.setSeriesName(1, F("Pin 2"));
	plot.getLineApperance(1)->setMarkerStyle(LineMarkers::CrossMarker);
	plot.getLineApperance(1)->setRGB(0x00ff00);

	plot.setSeriesName(2, F("Pin 3"));
	plot.getLineApperance(2)->setMarkerStyle(LineMarkers::CircleMarker);
	plot.getLineApperance(2)->setRGB(0x0000ff);
	
	//plot graph
	for(int i=0; i<=720; i+=5)
	{
		float c = cos(DEG_TO_RAD*(float)i); 
		float y1 = c + Dampen(i, ((float)(random(100)-50)) *0.1, 1, 1.0/240.0); 
		float y2 = c + Dampen(i, ((float)(random(100)-50)) *0.1, 1, 1.0/180.0)-2; 
		float y3 = c + Dampen(i, ((float)(random(100)-50)) *0.1, 1, 1.0/90.0)-4; 

		plot.plotDatumXYn(i, y1, y2, y3);
	}
	plot.finishGraph();
}

/** 
 * @example GNUPlot.h
 * Shows how to plot a bar graph.
 * @verbatim
 * Serial.begin([your baude rate here]);
 * GNUPlotBarGraph gnuBarPlotter(&Serial);
 * PlotSampleBarGraph(gnuBarPlotter);	
 * @endverbatim
 */
void PlotSampleBarGraph(SerialGraph& plot)
{
	//setup graph
	plot.newGraph();
	plot.enableSaveImageFile(true);
	plot.setTitle(F("Google Results for Instruments"));
	plot.setXAxisName(F("Instrument"));
	plot.setYAxisName(F("# Google Results (millions)"));
	plot.setShowGrid(true);

	//setup series
	plot.setSeriesName(0, F("play"));
	plot.getLineApperance(0)->setRGB(GraphColorsSchemeOrangeScale::Orange1);
	plot.setSeriesName(1, F("teach"));
	plot.getLineApperance(1)->setRGB(GraphColorsSchemeBlueScale::Blue1);
	plot.setSeriesName(2, F("like"));
	plot.getLineApperance(2)->setRGB(GraphColorsSchemeGreenScale::Green1);
	plot.setSeriesName(3, F("hate"));
	plot.getLineApperance(3)->setRGB(GraphColorsSchemeRedScale::Red1);

	//plot graph
	//Dataset compiled by Google-ing "I X the Y"
	//                instrument	play	teach	like	hate
	plot.plotDatumXYn(F("Piano"),		173,	25,		211,	21);
	plot.plotDatumXYn(F("Guitar"),		163,	22,		193,	27);
	plot.plotDatumXYn(F("Violin"),      41,		2,		43,		1);
	plot.plotDatumXYn(F("Drums"),		30,		8,		83,		9);
	
	plot.finishGraph();
}



/**
* Dampens another function (removes its energy), starting from x = 0
*
* @param x (tine)
* @param functionAtX some aritary value to de dampened.
* @param initialAmplidudeFactor (so 1 is a good value)
* @param lamda   The decay constant.
*
*/ 
float Dampen(float x, float functionAtX, float initialAmplidudeFactor, float lamda)
{
	return initialAmplidudeFactor * pow(M_E, -lamda * x) * functionAtX;
}

/**
* @param afreq      afreq s * pi * freq
*/
float SineWave(float xDeg, float afreq, float amplitude, float offsetDeg)
{
	return amplitude*sin(DEG_TO_RAD*xDeg + offsetDeg);
}


