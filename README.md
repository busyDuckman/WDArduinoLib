# WDArduinoLib  
A collection of useful Arduino helpers.  
  
Documentation here http://www.busyducks.com/downloads/doco/WDArduinoLib/index.html  
  
**Feature: ARDUINO GRAPHING LIBRARIES**  
GNUPlot based plotting.  
See http://busyducks.com/wp_4_1/2015/12/27/arduino-graphing-libraries/  
    
__Example__:  

![Example Graph](examples/Sine-Wave-with-Noise.png?raw=true "Example Graph")

    //main  
    GNUPlotPointGraph gnuPointPlotter(&Serial);  
    PlotPointNoise(gnuPointPlotter);  
  
    //method  
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
  
  