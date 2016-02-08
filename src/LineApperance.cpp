#include "LineApperance.h"

//--------------------------------------------------------------------------------------------------------------------------------------
void LineApperance::setRGB(long rgb)
{
	byte apperance = getApperanceByte();
	data = rgb;
	setApperanceByte(apperance);
}	

//--------------------------------------------------------------------------------------------------------------------------------------
LineStyles LineApperance::getLineStyle() const
{
	return (LineStyles) getBits<byte,int,2,0>(getApperanceByte());
}

//--------------------------------------------------------------------------------------------------------------------------------------
void LineApperance::setLineStyle(LineStyles style)
{
	byte b = getApperanceByte();
	setBits<2,1>(b, (byte)style);
	setApperanceByte(b);
}

//--------------------------------------------------------------------------------------------------------------------------------------
LineMarkers LineApperance::getMarkerStyle() const
{
	return (LineMarkers) getUnsignedInt<3,5>(getApperanceByte());
}

//--------------------------------------------------------------------------------------------------------------------------------------
void LineApperance::setMarkerStyle(LineMarkers marker)
{
	byte b = getApperanceByte();
	setBits<3, 5>(b, (byte)marker);
	setApperanceByte(b);
}

//--------------------------------------------------------------------------------------------------------------------------------------
int LineApperance::getLineWidth() const
{
	return getUnsignedInt<3,7>(getApperanceByte()) + 1;
}

//--------------------------------------------------------------------------------------------------------------------------------------
void LineApperance::setLineWidth(byte width)
{
	byte b = getApperanceByte();
	setBits<3,7>(b, width - 1);
	setApperanceByte(b);
}

//--------------------------------------------------------------------------------------------------------------------------------------
void LineApperance::printRGBAsHTMLCode(Print *outStream) const
{
	if(outStream == NULL)
	{
		return;
	}
	outStream->print(F("#"));
	printHexByte(outStream, getRed());
	printHexByte(outStream, getGreen());
	printHexByte(outStream, getBlue());
}
