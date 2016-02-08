/** 
 *  @file    GNUPlot.h
 *  @author  Warren Creemers (duckman)
 *  @date    22/12/2015
 *  @version 1.0 
 *  
 *  @copyright Copyright 2015 Dr Warren Creemers. All rights reserved.  See Licence.txt for details.
 *
 *  @brief GNUplot interface classes.
 */

#ifndef _GNUPLOT_h
#define _GNUPLOT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SerialGraph.h"
#include "BitArithmatic.h"
#include "LineApperance.h"
#include "StringValidation.h"
#include "Sanity.h"

//----------------------------------------------------------------------------------------------

/**
 * A base class for all things GNU plot.
 */
class GNUPlotBase : public SerialGraph
{
protected: 
	//---------------------------------------------------
	//server side variables

	//set a (gnuplot serverside) variables value
	template<typename T, typename U>
	void setVariable(T name, U value, bool quoted)
	{
		//title: set title "something" 
		outStream->print(F("set "));
		outStream->print(name);
		outStream->print(quoted ? F(" \"") : F(" "));
		outStream->print(value);
		outStream->println(quoted ? F("\"") : F(" "));
	}

	//just make (gnuplot serverside) varialbe exist, as a flag
	template<typename T>
	void setVariable(T name)
	{
		outStream->print(F("set "));
		outStream->println(name);			
	}

	//remove a (gnuplot serverside) varialbe
	template<typename T>
	void unsetVariable(T name)
	{
		//title: set title "something" 
		outStream->print(F("unset "));
		outStream->println(name);			
	}

	//sets a (gnuplot serverside) variable, or unsets it if the paramater is null
	template<typename T>
	void setVaribleByString_nullSafe(T name, const char *text)
	{
		if(text != NULL)
		{
			setVariable(name, text, true);
		}
		else
		{
			unsetVariable(name);
		}
	}

	//sets/unsets a (gnuplot serverside) variable
	template<typename T>
	void setVaribleFlagByBoolean(T name, bool value)
	{
		if(value)
		{
			setVariable(name);
		}
		else
		{
			unsetVariable(name);
		}
	}

protected:
	//---------------------------------------------------
	//server side variables

	//This should open an output, then cause a "replot"
	void savePlotToImageFile(const char* title);

	//Executed the first time a plot(...) command is called to plot data.
	//Its job is to prepare the script for insertion of data.
	void virtual makeReadyForPlotData();

	//Allocates memory with a usable filename for this graph (no extension).
	char* mallocBasicFileNameFromGraphTitle(); 

protected:
	//---------------------------------------------------
	// output final graph plott command

	//This method is called by finishGraph(), its job it to output the actual plot command to the terminal
	virtual void createGraph()=0;

	//helper method, called by createGraph
	void printTitleInPlotCommand(int seriesNum);

	//helper method, called by createGraph
	void printLineApperance(LineApperance apperance);

	//helper method, called by createGraph
	void printLogAxisCommand(const __FlashStringHelper* axisName, byte logScale);

public:
	//---------------------------------------------------
	// Constructor / Destructor


	GNUPlotBase(Print *_outStream) : SerialGraph(_outStream) {}
	virtual ~GNUPlotBase() { }
	//---------------------------------------------------
	// SerialGraph over-rides
	virtual void newGraph();
	virtual void finishGraph();

	virtual void printCommentStart() { outStream->print(F("#")); }

protected:
	//if I am plotting point x, y;  then this is the deliminator between x and y
	virtual void printSeperator() { outStream->print(F(", ")); }
	//if I am plotting point x, y;  then this is printed  before hand
	virtual void printStartDatum() { outStream->print(F("print ")); }
	//if I am plotting point x, y;  then this is printed  afterward
	virtual void printEndDatum() { outStream->println(); }

	virtual void _printDataValue(const char *value);

	virtual void _printDataValue(char value)
	{
		outStream->print(F("\"\\\"\", \""));
		outStream->print(value);
		outStream->print(F(" \", \"\\\"\""));
	}

	virtual void _printDataValue(bool value)
	{
		//true / false dont appear to be GNUplot constants.
		outStream->print(value ? 1 : 0);
	}
};

//----------------------------------------------------------------------------------------------
/**
 * A base class for all line and point plots.
 */
class GNUPlotXYGraphBase : public GNUPlotBase
{
public:
	GNUPlotXYGraphBase(Print *_outStream) : GNUPlotBase(_outStream) {}
	virtual ~GNUPlotXYGraphBase() { }
	//virtual GraphStyle getGraphStyle() = 0;
	virtual void createGraph();
};


//----------------------------------------------------------------------------------------------
/**
 * Specilisation of GNUPlotXYGraphBase to make a nice line plot.
 * GNUPlotLineGraph::getGraphStyle() alters the behaviour of GNUPlotXYGraphBase::createGraph()
 */
class GNUPlotLineGraph : public GNUPlotXYGraphBase
{
public:
	GNUPlotLineGraph(Print *_outStream) : GNUPlotXYGraphBase(_outStream) {}
	virtual ~GNUPlotLineGraph() { }
protected:
	virtual GraphStyle getGraphStyle() const { return GraphStyle::LinePlot; }
};

//----------------------------------------------------------------------------------------------
/**
 * Specilisation of GNUPlotXYGraphBase to make a nice scatter plot.
 * GNUPlotPointGraph::getGraphStyle() alters the behaviour of GNUPlotXYGraphBase::createGraph()
 */
class GNUPlotPointGraph : public GNUPlotXYGraphBase
{
public:
	GNUPlotPointGraph(Print *_outStream) : GNUPlotXYGraphBase(_outStream) {}
	virtual ~GNUPlotPointGraph() { }
	virtual GraphStyle getGraphStyle() const { return GraphStyle::PointPlot; }
};

//----------------------------------------------------------------------------------------------
/**
 * Specilisation of GNUPlotBase to make bar graphs.
 */
class GNUPlotBarGraph : public GNUPlotBase
{
public:
	GNUPlotBarGraph(Print *_outStream) : GNUPlotBase(_outStream) {}
	virtual ~GNUPlotBarGraph() { }
	virtual GraphStyle getGraphStyle() const { return GraphStyle::BarGraph; }
protected:
	virtual void createGraph();
};


#endif //_GNUPLOT_h