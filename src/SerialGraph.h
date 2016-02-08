// SerialGraph.h

/** 
 *  @file    .h
 *  @author  Warren Creemers (duckman)
 *  @date    22/12/2015
 *  @version 1.0 
 *  
 *  @copyright Copyright 2015 Dr Warren Creemers. All rights reserved.  See Licence.txt for details.
 *
 *  @brief 
 */


#ifndef _SERIALGRAPH_h
#define _SERIALGRAPH_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "StringValidation.h"
#include "BitArithmatic.h"
#include "LineApperance.h"
#include "Sanity.h"

//----------------------------------------------------------------------------------------------
/**
 * Graph type enumeration.
 * Used internally, the user does not need to employ this enumeration.
 */
enum GraphStyle {LinePlot, PointPlot, BarGraph};

//----------------------------------------------------------------------------------------------
/** 
 * A base class for creating graphs via terminal commands issues via a device inheriting from 
 * print (comm port, file, telnet).
 * 
 * I could see no good reason for extensive getters, as this is a 'setup and and execute' type class.
 *
 * @note Some dynamic memory usage, The caller should refrain from using malloc during plotting 
 *       operations to prevent possible heap fragmentation. Dynamic memory usage is a design decision 
 *       I made weighing pro's and con's of the situation. Given 'typical usage' scenarios of the API
 *       it should not cause heap fragmentation.
 *
 * @note Starting "another plot" , I strongly recomend you call newGraph() first, then change the 
 *       series names and other test labels. This will free all labels, preventing memory
 *       fragmentation.
 */ 
class SerialGraph
{
		
protected:
	//--------------------------------
	// Instance data
	//--------------------------------

	/** True if an image file should be saved */
	bool saveFile;
	/** The script to write to. Bad things happen if this is null. */
	Print *outStream;		
		
	/** If this is true, then x co-ordinates are being supplied via the plot command, (ohterwise we will autogenerate it in GNUPlot). */
	bool xAxisSpecified;	
	/** Actual number of YSeries being used. The consts maxSeries and maxStringSize may need tweaking. */
	int numYSeries;			

	/** Places a grid on the plot. */
	bool showGrid;         

	/** The graphs title (optional), see maxStringSize. */
	char *graphTitle; 
	/** x axis name (optional), see maxStringSize */
	char *xAxisName;       
	/** y axis name (optional), see maxStringSize */
	char *yAxisName;        

	/** name of every series (optional), see maxStringSize. */
	char **seriesNames;            
	/** Apperance of every series, see maxStringSize. NB: 0 = solid black line, 1 unit wide */
	LineApperance *seriesApperance;  

	/** 0 = no log; 2 or more sets the log scale. */
	byte xAxisLogScale;		
	/** 0 = no log; 2 or more sets the log scale. */
	byte yAxisLogScale;    

public:	
	/** Sets the maximum string size (titles, series anmes etc). */
	const byte maxStringSize = 128;
	/** Number of possible series. (at least 8 bytes used per series, so only increase this number if you have to) */
	const byte maxSeries = 8;

	//--------------------------------
	// Implement these
	//--------------------------------
	/** Returns the GraphStyle of the current object. */
	virtual GraphStyle getGraphStyle() const = 0;

	/**
	 * Serial output that establishes a new graph.
	 * This should also call init(), to reset the class.
	 * @note: If you wan't a "memory breather" between plots, calling this method frees all series names, titles etc.
	 */
	virtual void newGraph() = 0;
		
	/** Serial output that finishes the plot, saves file, updates display etc. */
	virtual void finishGraph() = 0;

	/** Prints the text which makes a line a comment */
	virtual void printCommentStart() = 0;

protected:
	/** If I am plotting x, y1, y2;  then this is the deliminator (the comma in this case) */
	virtual void printSeperator() = 0;
		
	/** If I am plotting point x, y;  then this is printed  before hand. */
	virtual void printStartDatum() = 0;

	/**
	 *   If I am plotting point x, y; then this is printed  afterward.
	 *   It should include outStream->println(); if data is to be sent 
	 *   via seperate lines.
	 */
	virtual void printEndDatum() = 0;

	//--------------------------------
	// Accessors
	//--------------------------------
public:	
	/**
	 * Set to true if an image file should be saved. 
	 * @note Filename will be based on graph title. This is done to save memory.
	 *       Final filename has some extra validation performed to conform with
	 *       filename rules aplicable to modern operating systems (excluding 8 char limit).
	 */
	virtual void enableSaveImageFile(bool enabled) { saveFile = enabled; }
		
	/**
	 * Set to place a grid on the plot. 
	 * @note Grid size is automatic.
	 */
	virtual void setShowGrid(bool enabled) { showGrid = enabled; }
	
	/**
	 * Sets the graph title (optional).
     * Validation is performed on the name (NoQuotes, ComPortSafe, NoNewLines), also enforces maxStringSize.
	 */
	void setTitle(const __FlashStringHelper* title) 
	{
		//buffer may be NULL, if title is NULL
		char *buffer = mallocCopyOf(title);
		setTitle(buffer);
		if(buffer != NULL)
		{
			free(buffer);
		}
	}
	
	/**
	 * Sets the graph title (optional).
     * Validation is performed on the name (NoQuotes, ComPortSafe, NoNewLines), also enforces maxStringSize.
	 */
	virtual void setTitle(const char* title)
	{
		if(graphTitle != NULL)
		{
			free(graphTitle);
		}
		graphTitle = mallocSanitizedCopy(title, StringValidationRules::NoQuotes | 
												StringValidationRules::ComPortSafe | 
												StringValidationRules::NoNewLines, 
												maxStringSize);
	}

	//--------------------------------
	// Axis related accessors
	//--------------------------------
	/**
	 * Sets x axis name (optional).
     * Validation is performed on the name (NoQuotes, ComPortSafe, NoNewLines), also enforces maxStringSize.
	 */
	void setXAxisName(const __FlashStringHelper* title) 
	{
		//buffer may be NULL, if title is NULL
		char *buffer = mallocCopyOf(title);
		setXAxisName(buffer);
		if(buffer != NULL)
		{
			free(buffer);
		}
	}

	/**
	 * Sets x axis name (optional).
     * Validation is performed on the name (NoQuotes, ComPortSafe, NoNewLines), also enforces maxStringSize.
	 */
	virtual void setXAxisName(const char* name)
	{
		if(xAxisName != NULL)
		{
			free(xAxisName);
		}
		xAxisName = mallocSanitizedCopy(name, StringValidationRules::NoQuotes | StringValidationRules::ComPortSafe | StringValidationRules::NoNewLines, maxStringSize);
	}

	/**
	 * Sets y axis name (optional).
     * Validation is performed on the name (NoQuotes, ComPortSafe, NoNewLines), also enforces maxStringSize.
	 */
	void setYAxisName(const __FlashStringHelper* title) 
	{
		//buffer may be NULL, if title is NULL
		char *buffer = mallocCopyOf(title);
		setYAxisName(buffer);
		if(buffer != NULL)
		{
			free(buffer);
		}
	}

	/**
	 * Sets y axis name (optional).
     * Validation is performed on the name (NoQuotes, ComPortSafe, NoNewLines), also enforces maxStringSize.
	 */
	virtual void setYAxisName(const char* name)
	{
		if(yAxisName != NULL)
		{
			free(yAxisName);
		}
		yAxisName = mallocSanitizedCopy(name, StringValidationRules::NoQuotes | StringValidationRules::ComPortSafe | StringValidationRules::NoNewLines, maxStringSize);
	}

	/**
	* @param scale   The log scale, eg 2 for binery, 10 for common logarithm.
	*                Use 1 or 0 to disable log scale.
	*/
	virtual void setXAxisLog(byte scale)
	{
		xAxisLogScale = (scale == 1) ? 0 : scale;
	}

	/**
	* @param scale   The log scale, eg 2 for binery, 10 for common logarithm.
	*                Use 1 or 0 to disable log scale.
	*/
	virtual void setYAxisLog(byte scale)
	{
		yAxisLogScale = (scale == 1) ? 0 : scale;
	}

	//--------------------------------
	// Series related accessors
	//--------------------------------
	/**
	 * Sets the name of a series. 
	 * Validation is performed on the name (NoQuotes, ComPortSafe, NoNewLines), also enforces maxStringSize.
	 *
	 * @param title  The title.
	 * @param _n     Series number to alter.
	 *
	 * @note  Behaviour on _n > maxSeries, is to just override the last possible series entry.
	 */
	void setSeriesName(int _n, const __FlashStringHelper* title) 
	{
		//buffer may be NULL, if title is NULL
		char *buffer = mallocCopyOf(title);
		setSeriesName(_n, buffer);
		if(buffer != NULL)
		{
			free(buffer);
		}
	}

	/**
	 * Sets the name of a series. 
	 * Validation is performed on the name (NoQuotes, ComPortSafe, NoNewLines), also enforces maxStringSize.
	 *
	 * @param title  The title.
	 * @param _n     Series number to alter.
	 *
	 * @note  Behaviour on _n > maxSeries, is to just override the last possible series entry.
	 */
	virtual void setSeriesName(int _n, const char* name)
	{
		//behaviour on n > maxSeries, is to just override the last possible series
		int n = max(0, ((_n > maxSeries) ? maxSeries : _n));

		if(seriesNames[n] != NULL)
		{
			free(seriesNames[n]);
		}
		seriesNames[n] = mallocSanitizedCopy(name, StringValidationRules::NoQuotes | StringValidationRules::ComPortSafe | StringValidationRules::NoNewLines, maxStringSize);
	}

	/**
	 * Gets a structure that controls apperance of a series (called a LineApperance). 
	 * It is intended that you directly modify the structure returned, there is no setLineApperance.
	 * See the struct LineApperance for more. 
	 *
	 * @param _n    Series number to alter.
	 *
	 * @note  Behaviour on _n > maxSeries, is to just override the last possible series entry.
	 */
	LineApperance* getLineApperance(int _n)
	{
		//behaviour on n > maxSeries, is to just override the last possible series
		int n = max(0, ((_n > maxSeries) ? maxSeries : _n));

		return &seriesApperance[n];
	}

	//--------------------------------
	// Constructor / destructor.
	//--------------------------------
   /**
	* Constructor.
	* @param _outStream    The stream to which the (script for creating a?) graph is written.
	*/
	SerialGraph(Print *_outStream);

  /**
    * Destructor.
	* I am not telling you to use it, I don't want to debate how to employ the API.
	* If you want to dispose of the graph, this will do so, releasing all dynamicaly allocated memory.
	* 
	* @note: If you wan't a "memory breather" between plots, consider calling newGraph(), it frees all series names, titles etc.
	*/
	virtual ~SerialGraph();

protected:

   /**
    * Brings the class to its default state.
	* Called by the constructor and newGraph().
	*/
	void init();

	//---------------------------------------------------------------------
	// Related to prparing to recive plot data

	/**
	 * Called by every plot command prior to sending data.
	 * it's function is to call makeReadyForPlotData() the first time it is called.
	 * @param value		What to print.
	 */
	void ensureReadyToRecivePlotData();

   /**
    * Called by ensureReadyToRecivePlotData, if the first plot command is encountered.
    * @note A concrete class should override this.
	*/
	virtual void makeReadyForPlotData();

protected:
	//---------------------------------------------------------------------
	// Sending data

	/**
	 * Prints plot data in a format that the server can understand.
	 * Override a coresponding  _printDataValue(...) to put appropriate 
	 * quotes around strings etc.
	 *
	 *
	 * @param value		What to print.
	 */
	template<typename T>
	void printDataValue(T value)
	{
		_printDataValue(value);
	}

	/**
	 * Override a put appropriate quotes around strings etc.
	 * @param value		What to print.
	 */
	virtual void _printDataValue(const char *value)
	{
		outStream->print(value);
	}

	/**
	 * This just calls _printDataValue(const char *value) so probably alter that instead.
	 * @param value		What to print.
	 */
	virtual void _printDataValue(const __FlashStringHelper* value)
	{
		char *buffer = mallocCopyOf(value);
		outStream->print(buffer);
		free(buffer);
	}

	/**
	 * Override for customised value output.
	 * @param value		What to print.
	 */
	virtual void _printDataValue(char value)
	{
		outStream->print(value);
	}

	/**
	 * Override for customised value output.
	 * @param value		What to print.
	 */
	virtual void _printDataValue(int value)
	{
		outStream->print(value);
	}

	/**
	 * Override for customised value output.
	 * @param value		What to print.
	 */
	virtual void _printDataValue(long value)
	{
		outStream->print(value);
	}

	/**
	 * Override for customised value output.
	 * @param value		What to print.
	 */
	virtual void _printDataValue(byte value)
	{
		outStream->print(value);
	}

	/**
	 * Override for customised value output.
	 * @param value		What to print.
	 */
	virtual void _printDataValue(unsigned int value)
	{
		outStream->print(value);
	}

	/**
	 * Override for customised value output.
	 * @param value		What to print.
	 */
	virtual void _printDataValue(unsigned long value)
	{
		outStream->print(value);
	}

	/**
	 * Override for customised value output.
	 * @param value		What to print.
	 */
	virtual void _printDataValue(bool value)
	{
		outStream->print(value);
	}

	/**
	 * Override for customised value output.
	 * @param value		What to print.
	 */
	virtual void _printDataValue(double value)
	{
		outStream->print(value);
	}

public:

	/**
	 * Outputs a point to be plotted.
	 * All Y values must be of the same type.
	 *
	 * @note  The first time this is called after newGraph(), makeReadyForPlotData() will be called.
	 * @note  The following feilds automatically set: xAxisSpecified; numYSeries;
	 */
	template<typename YT>
	void plotDatumY(YT y1) 
	{
		ensureReadyToRecivePlotData();
		xAxisSpecified=false; numYSeries=1;
		printStartDatum(); outStream->print(y1); printEndDatum();
	}

	/**
	 * Outputs a point to be plotted.
	 * All Y values must be of the same type.
	 *
	 * @note  The first time this is called after newGraph(), makeReadyForPlotData() will be called.
	 * @note  The following feilds automatically set: xAxisSpecified; numYSeries;
	 */
	template<typename XT, typename YT>
	void plotDatumXY(XT x, YT y1) 
	{
		ensureReadyToRecivePlotData();
		xAxisSpecified=true; numYSeries=1;
		printStartDatum(); printDataValue(x); printSeperator(); printDataValue(y1); printEndDatum();
	}
	
	/**
	 * Outputs a point to be plotted.
	 * All Y values must be of the same type.
	 *
	 * @note  The first time this is called after newGraph(), makeReadyForPlotData() will be called.
	 * @note  The following feilds automatically set: xAxisSpecified; numYSeries;
	 */
	template<typename YT>
	void plotDatumYn(YT y1, YT y2) 
	{
		ensureReadyToRecivePlotData();
		xAxisSpecified=false; numYSeries=2;
		printStartDatum(); printDataValue(y1); printSeperator(); printDataValue(y2); printEndDatum();
	}

	/**
	 * Outputs a point to be plotted.
	 * All Y values must be of the same type.
	 *
	 * @note  The first time this is called after newGraph(), makeReadyForPlotData() will be called.
	 * @note  The following feilds automatically set: xAxisSpecified; numYSeries;
	 */
	template<typename YT>
	void plotDatumYn(YT y1, YT y2, YT y3)
	{
		ensureReadyToRecivePlotData();
		xAxisSpecified=false; numYSeries=3;
		printStartDatum(); 
		printDataValue(y1); printSeperator(); 
		printDataValue(y2); printSeperator(); 
		printDataValue(y3);
		printEndDatum();
	}

	/**
	 * Outputs a point to be plotted.
	 * All Y values must be of the same type.
	 *
	 * @note  The first time this is called after newGraph(), makeReadyForPlotData() will be called.
	 * @note  The following feilds automatically set: xAxisSpecified; numYSeries;
	 */
	template<typename YT>
	void plotDatumYn(YT y1, YT y2, YT y3, YT y4)
	{
		ensureReadyToRecivePlotData();
		xAxisSpecified=false; numYSeries=4;
		printStartDatum(); 
		printDataValue(y1); printSeperator(); 
		printDataValue(y2); printSeperator(); 
		printDataValue(y3); printSeperator(); 
		printDataValue(y4);
		printEndDatum();
	}

	/**
	 * Outputs a point to be plotted.
	 * All Y values must be of the same type.
	 *
	 * @note  The first time this is called after newGraph(), makeReadyForPlotData() will be called.
	 * @note  The following feilds automatically set: xAxisSpecified; numYSeries;
	 */
	template<typename YT>
	void plotDatumYn(YT y1, YT y2, YT y3, YT y4, YT y5) 
	{
		ensureReadyToRecivePlotData();
		xAxisSpecified=false; numYSeries=5;
		printStartDatum(); 
		printDataValue(y1); printSeperator(); 
		printDataValue(y2); printSeperator(); 
		printDataValue(y3); printSeperator(); 
		printDataValue(y4); printSeperator(); 
		printDataValue(y5); 
		printEndDatum();
	}

	/**
	 * Outputs a point to be plotted.
	 * All Y values must be of the same type.
	 *
	 * @note  The first time this is called after newGraph(), makeReadyForPlotData() will be called.
	 * @note  The following feilds automatically set: xAxisSpecified; numYSeries;
	 */
	template<typename XT, typename YT>
	void plotDatumXYn(XT x, YT y1, YT y2) 
	{ 
		ensureReadyToRecivePlotData();
		xAxisSpecified=true; numYSeries=2;
		printStartDatum(); printDataValue(x); printSeperator(); printDataValue(y1); printSeperator(); printDataValue(y2); printEndDatum();
	}

	/**
	 * Outputs a point to be plotted.
	 * All Y values must be of the same type.
	 *
	 * @note  The first time this is called after newGraph(), makeReadyForPlotData() will be called.
	 * @note  The following feilds automatically set: xAxisSpecified; numYSeries;
	 */
	template<typename XT, typename YT>
	void plotDatumXYn(XT x, YT y1, YT y2, YT y3) 
	{ 
		ensureReadyToRecivePlotData();
		xAxisSpecified=true; 
		numYSeries=3;
		printStartDatum(); 
		printDataValue(x); printSeperator(); 
		printDataValue(y1); printSeperator(); 
		printDataValue(y2); printSeperator(); 
		printDataValue(y3);
		printEndDatum();
	}

	/**
	 * Outputs a point to be plotted.
	 * All Y values must be of the same type.
	 *
	 * @note  The first time this is called after newGraph(), makeReadyForPlotData() will be called.
	 * @note  The following feilds automatically set: xAxisSpecified; numYSeries;
	 */
	template<typename XT, typename YT>
	void plotDatumXYn(XT x, YT y1, YT y2, YT y3, YT y4) 
	{ 
		ensureReadyToRecivePlotData();
		xAxisSpecified=true; 
		numYSeries=4;
		printStartDatum(); 
		printDataValue(x); printSeperator(); 
		printDataValue(y1); printSeperator(); 
		printDataValue(y2); printSeperator(); 
		printDataValue(y3); printSeperator(); 
		printDataValue(y4);
		printEndDatum();
	}
	
	/**
	 * Outputs a point to be plotted.
	 * All Y values must be of the same type.
	 *
	 * @note  The first time this is called after newGraph(), makeReadyForPlotData() will be called.
	 * @note  The following feilds automatically set: xAxisSpecified; numYSeries;
	 */
	template<typename XT, typename YT>
	void plotDatumXYn(XT x, YT y1, YT y2, YT y3, YT y4, YT y5) 
	{ 
		ensureReadyToRecivePlotData();
		xAxisSpecified=true; 
		numYSeries=5;
		printStartDatum(); 
		printDataValue(x); printSeperator(); 
		printDataValue(y1); printSeperator(); 
		printDataValue(y2); printSeperator(); 
		printDataValue(y3); printSeperator(); 
		printDataValue(y4); printSeperator(); 
		printDataValue(y5); 
		printEndDatum();
	}

	/**
	 * Outputs a point to be plotted.
	 * All Y values must be of the same type.
	 *
	 * @param yVec  Pointer to an array of yValues.
	 * @param _n    Number of items in array.
	 *
	 * @note  Behaviour on _n > maxSeries, is to just override the last possible series entry.
	 */
	template<typename XT, typename YT>
	void plotDatumXYn(XT x, const YT *yVec, int _n) 
	{ 
		ensureReadyToRecivePlotData();
		//behaviour on n > maxSeries, is to just override the last possible series
		int n = max(0, ((_n > maxSeries) ? maxSeries : _n));
		xAxisSpecified=true; numYSeries=n;
		printStartDatum();
		printDataValue(x);
		printSeperator();
			
		for(int i=0; i<n; i++)
		{
			printDataValue(yVec[i]);
			printSeperator();
		}
		printEndDatum();
	}

	/**
	 * Outputs a point to be plotted.
	 * All Y values must be of the same type.
	 *
	 * @param yVec  Pointer to an array of yValues.
	 * @param _n    Number of items in array.
	 *
	 * @note  Behaviour on _n > maxSeries, is to just override the last possible series entry.
	 */
	template<typename XT, typename YT>
	void plotDatumYn(const YT *yVec, int _n) 
	{ 
		ensureReadyToRecivePlotData();
		//behaviour on n > maxSeries, is to just override the last possible series
		int n = max(0, ((_n > maxSeries) ? maxSeries : _n));
		xAxisSpecified=false; numYSeries=n;
		printStartDatum();

		for(int i=0; i<n; i++)
		{
			printDataValue(yVec[i]);
			printSeperator();
		}
		printEndDatum();
	}

	//---------------------------------------------------------------
	// Sending Coments

	/**
	 * Outputs text directly to the graph output.
	 * To use this for debug notes etc, call printCommentStart() first.
	 *
	 * @param  what Anything you can sent to a Print class.
	 * @note        No string validation; as this is not considered a 
	 *              method that should recive data from outside the
	 *              codebase.
	 */
	template<typename WT>
	void print(const WT what)
	{
		outStream->print(what);
	}

	/**
	 * Outputs text directly to the graph output.
	 * To use this for debug notes etc, call printCommentStart() first.
	 *
	 * @param  what Anything you can sent to a Print class.
	 * @note        No string validation; as this is not considered a 
	 *              method that should recive data from outside the
	 *              codebase.
	 */
	template<typename WT>
	void println(const WT what)
	{
		outStream->println(what);
	}

    /**
	 * Outputs a comment that is ignored by the graph output.
	 * Use this for debug notes etc.
	 *
	 * @param what Anything you can sent to a Print class.
	 * @note        No string validation; as this is not considered a 
	 *              method that should recive data from outside the
	 *              codebase.
	 */
	template<typename WT>
	void printComment(const WT what)
	{
		printCommentStart();
		outStream->println(what);
	}

	/**
	 * Outputs a comment that is ignored by the graph output.
	 * Use this for debug notes etc.
	 *
	 * @param what Anything you can sent to a Print class.
	 * @note        No string validation; as this is not considered a 
	 *              method that should recive data from outside the
	 *              codebase.
	 */
	template<typename WT>
	void printComment(const WT *what)
	{
		printCommentStart();
		if(what != NULL)
		{
			outStream->println(what);
		}
	}

};

#endif //_SERIALGRAPH_h

