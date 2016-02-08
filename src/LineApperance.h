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

#ifndef _LINEAPPERANCE_h
#define _LINEAPPERANCE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "BitArithmatic.h"


/**
 * @public
 * Part of the GraphColorsScheme*Scale colour schemes. They should be used to with data where the colour signifies importance.(NB: Green1 is bolder than Green2 etc.)
 * seealso ::GraphColorsSchemeGreyScale, ::GraphColorsSchemeRedScale, ::GraphColorsSchemeGreenScale, ::GraphColorsSchemeBlueScale, ::GraphColorsSchemeOrangeScale, ::GraphColorsSchemePurpleScale
 * NB: Used a tool at http://colorbrewer2.org/ to create GraphColorsScheme*Scale shemes. 
 * @note Red1 is balanced to work with Green1, Blue1, Purple1 etc. This is a method to find colors that work together.
 */
enum GraphColorsSchemeGreyScale    {GreyScale5=0xf7f7f7, GreyScale4=0xcccccc, GreyScale3=0x969696, GreyScale2=0x636363, GreyScale1=0x252525};
/** @copydoc ::GraphColorsSchemeGreyScale */
enum GraphColorsSchemeRedScale     {Red5=0xfee5d9, Red4=0xfcae91, Red3=0xfb6a4a, Red2=0xde2d26, Red1=0xa50f15};
/** @copydoc ::GraphColorsSchemeGreyScale */
enum GraphColorsSchemeGreenScale    {Green5=0xedf8e9, Green4=0xbae4b3, Green3=0x74c476, Green2=0x31a354, Green1=0x006d2c};
/** @copydoc ::GraphColorsSchemeGreyScale */
enum GraphColorsSchemeBlueScale     {Blue5=0xeff3ff, Blue4=0xbdd7e7, Blue3=0x6baed6, Blue2=0x3182bd, Blue1=0x08519c};
/** @copydoc ::GraphColorsSchemeGreyScale */
enum GraphColorsSchemeOrangeScale   {Orange5=0xfeedde, Orange4=0xfdbe85, Orange3=0xfd8d3c, Orange2=0xe6550d, Orange1=0xa63603};
/** @copydoc ::GraphColorsSchemeGreyScale */
enum GraphColorsSchemePurpleScale   {Purple5=0xf2f0f7, Purple4=0xcbc9e2, Purple3=0x9e9ac8, Purple2=0x756bb1, Purple1=0x54278f};


/** @public
 * 12 points of the spectrum from red to blue at full saturation with 37.5% luminosity. 
 * This configuration is intended to produce good results printing spectral colours against white,
 * because the yellow comes out olive, and the aqua comes out teal.
 * The down side being no vibrant orange. It's a trade off that makes sense for graphs.
 * If you are looking for colors that specificaly don't imply one catagory is more important see ::CategoricalSchemeStrong instead.
 * NB: This is an original pallate design by the author.
 *
 * @note Not designed for use with the GraphColorsScheme*Scale colour schemes.
 */
enum GraphColorsSchemeSpectral {Spectral1=0xc00000, Spectral2=0xc04800, Spectral3=0xc09000, Spectral4=0xa8c000, Spectral5=0x60c000, Spectral6=0x18c000, 
                                Spectral7=0x00c030, Spectral8=0x00c078, Spectral9=0x00c0c0, Spectral10=0x0078c0, Spectral11=0x0030c0, Spectral12=0x1800c0};


/** @public
 * These colours look equally strong, for when no category is more important.
 * NB: working from information at  https://www.perceptualedge.com/articles/b-eye/choosing_colors.pdf and http://colorbrewer2.org/
 * @note Not designed for use with the GraphColorsScheme*Scale colour schemes.
 */
enum CategoricalSchemeStrong { CatBlack=0x000000, CatRed=0xe41a1c, CatBlue=0x377eb8, CatGreen=0x4daf4a, CatPurple=0x984ea3, CatOrange=0xff7f00, 
                               CatYellow=0xffff33, CatBrown=0xa65628, CatLilac=0xf781bf};


/** @public
 * Compromising aesthetics, for a scheme that anyone should be able to make out.
 * NB: working from information at http://colorbrewer2.org/
 * @note Not designed for use with the GraphColorsScheme*Scale colour schemes.
 */
enum ColorBlindSafe { ColBlind1=0xb2182b, ColBlind2=0xef8a62, ColBlind3=0xfddbc7, ColBlind4=0x67a9cf, ColBlind5=0x2166ac, ColBlind6=0x000000, ColBlind7=0xd1e5f0};

/** @public
 *  The type of lines that can be plotted.
 *  Not aplicable to bar graphs, or point graphs.
 */
enum LineStyles {SolidLine=0, DashedLine, DotedLine, DashDotLine};

/** @public
 *  The type of point markers that can be plotted.
 *  Not aplicable to bar graphs.
 */
enum LineMarkers {NoMarker=0,  CircleMarker,      SquareMarker,      DiamondMarker, 
	              CrossMarker, SolidCircleMarker, SolidSquareMarker, SolidDiamondMarker};

//----------------------------------------------------------------------------------------------
// LineApperance

/**
 * A struct to hold a series apperance (refered to as a Line Apperance).
 * 
 * This struct is bit compact and uses 32 bits of data (stored as uint32_t).
 *
 * @verbatim
 * Memory map af LineApperance [bits]
 * 0123456789ABCDEF0123456789ABCDEF  
 * XX      |       |       |         (2 bits) one of 4 line styles
 *   XXX   |       |       |         (3 bits) one of 8 line markers
 *      XXX|       |       |         (3 bits) line width (1 to 8)
 *         XXXXXXXX|       |         (1 byte) red
 *         |       XXXXXXXX|         (1 byte) green
 *         |       |       XXXXXXXX  (1 byte) blue
 * @endverbatim
 */
struct LineApperance
{
public:
   /**
	* Memory for the packed bits.
	*/
	uint32_t data;

   /**
	* Set data to 0. This creates a solid black line no marker, size 1.
	*/
	void init()
	{
		data = 0; //solid black line no marker, size 1. Nifty.
	}

   /**
    * Gets the red component 0-255.
	* @return the red component 0-255
	*/
	byte getRed() const {return ((byte*) &data)[2];  }

   /**
	* Sets the red component 0-255.
	* @param r The red component.
	*/
	void setRed(byte r) {((byte*) &data)[2] = r; }

   /**
    * Gets the green component 0-255.
	* @return the green component 0-255
	*/
	byte getGreen() const {return ((byte*) &data)[1]; }

   /**
    * Gets the green component 0-255.
	* @return the green component 0-255
	*/
	void setGreen(byte g) {((byte*) &data)[1] = g; }
   
   /**
    * Gets the blue component 0-255.
	* @return the blue component 0-255
	*/
	byte getBlue() const {return ((byte*) &data)[0]; }

   /**
    * Gets the blue component 0-255.
	* @return the blue component 0-255
	*/
	void setBlue(byte b) { ((byte*) &data)[0] = b; }

   /**
    * Gets a 24bit rgb value of the current color.
	* @return 24bit rgb value of the current color
	*/
	long getRGB() const {return data & 0x00ffffffL;}

   /**
    * Sets a 24bit rgb value of the current color.
	* @param rgb 24bit rgb value of the current color. Highest byte ignored.
	*/
	void setRGB(long rgb);

   /**
    * Gets the line style.
	* @return  the current line style, see ::LineStyles. 
	*/
	LineStyles getLineStyle() const;

   /**
    * Sets the line style.
	* @param style The line style, see ::LineStyles. 
	*/
	void setLineStyle(LineStyles style);

   /**
    * Gets the marker style.
	* @return  the current marker style, see ::LineMarkers. 
	*/
	LineMarkers getMarkerStyle() const;

   /**
    * Sets the marker style.
	* @param marker The marker style, see ::LineMarkers. 
	*/
	void setMarkerStyle(LineMarkers marker);

   /**
    * Gets the line width.
	* @return  the current line width (1 to 8). 
	*/
	int getLineWidth() const;

   /**
    * Sets the line width.
	* @return  the current line width (1 to 8). 
	* @note Setting width to 0, sets maximum width. 0 width lines are not allowed.
	*/
	void setLineWidth(byte width);

   /**
    * Prints the current color as a html code. 
	* @note as an example, output looks like: "#FF0070" where red = FF.
	* @param outStream Stream to print to. Can be NULL to disable.
	*/
	void printRGBAsHTMLCode(Print *outStream) const;

private:
	inline byte getApperanceByte() const {return ((byte*) &data)[3]; }
	inline void setApperanceByte(byte b) { ((byte*) &data)[3] = b; }
};

#endif //_LINEAPPERANCE_h
