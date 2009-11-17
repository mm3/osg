/**********************************************************************
 *
 *    FILE:            Text.cpp
 *
 *    DESCRIPTION:    Read/Write osgText::Text in binary format to disk.
 *
 *    CREATED BY:        Auto generated by iveGenerator
 *                    and later modified by Rune Schmidt Jensen.
 *
 *    HISTORY:        Created 27.3.2003
 *
 *    Copyright 2003 VR-C
 **********************************************************************/

#include "Exception.h"
#include "Text.h"
#include "Drawable.h"
#include "Object.h"

#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osg/Notify>

using namespace ive;

void Text::write(DataOutputStream* out){
    // Write Text's identification.
    out->writeInt(IVETEXT);
    // If the osg class is inherited by any other class we should also write this to file.
    osg::Object*  obj = dynamic_cast<osg::Object*>(this);
    if(obj){
        ((ive::Drawable*)(obj))->write(out);
    }
    else
        out_THROW_EXCEPTION("Text::write(): Could not cast this osgText::Text to an osg::Drawable.");
    
    // Write Text's properties.
    if( getFont() )
    {
        std::string fname = getFont()->getFileName();

        if(!fname.empty())
        {
            if(out->getUseOriginalExternalReferences())
            {
                out->writeString(fname); //Saving file name with local directory
            }
            else
            {
                out->writeString(osgDB::getSimpleFileName(fname)); //Saving original file name
            }
        }
        else
            out->writeString(""); //Blank string
    }
    else
        out->writeString(""); //Blank string

    out->writeUInt(getFontWidth());
    out->writeUInt(getFontHeight());
    out->writeFloat(getCharacterHeight());
    out->writeFloat(getCharacterAspectRatio());
    out->writeUInt(getCharacterSizeMode());
    out->writeFloat(getMaximumWidth());
    out->writeFloat(getMaximumHeight());
    
    out->writeFloat(getLineSpacing());
    
    out->writeUInt(getAlignment());

    out->writeQuat(getRotation()); //FIXME: controllare che ci sia

    out->writeBool(getAutoRotateToScreen());
    out->writeUInt(getLayout());
    out->writeVec3(getPosition());
    out->writeVec4(getColor());
    out->writeUInt(getDrawMode());

    if ( out->getVersion() >= VERSION_0041 )
    {
        out->writeFloat(getBoundingBoxMargin());
        out->writeVec4(getBoundingBoxColor());
    }

    if ( out->getVersion() >= VERSION_0028 )
    {
        out->writeUInt(getBackdropType());

        out->writeFloat(getBackdropHorizontalOffset());
        out->writeFloat(getBackdropVerticalOffset());

        out->writeVec4(getBackdropColor());
        out->writeUInt(getBackdropImplementation());

        out->writeUInt(getColorGradientMode());
        out->writeVec4(getColorGradientTopLeft());
        out->writeVec4(getColorGradientBottomLeft());
        out->writeVec4(getColorGradientBottomRight());
        out->writeVec4(getColorGradientTopRight());
    }

    // text :: Modified from osgPlugins::osg
    const osgText::String& textstring = getText();
    bool isACString = true;
    osgText::String::const_iterator itr;
    for(itr=textstring.begin();
        itr!=textstring.end() && isACString;
        ++itr)
    {
        if (*itr==0 || *itr>256) isACString=false;
    }

    if (isACString)
    {
        std::string str;

        for(itr=textstring.begin();
            itr!=textstring.end();
            ++itr)
        {
            str += (char)(*itr);
        }

        //std::copy(textstring.begin(),textstring.end(),std::back_inserter(str));
        
        out->writeBool(true);
        out->writeString(str);
    }
    else
    {
        // do it the hardway...output each character as an int
        osg::ref_ptr<osg::UIntArray> strarr = new osg::UIntArray(textstring.size());
        
        for(itr=textstring.begin();
            itr!=textstring.end();
            ++itr)
        {
            strarr->push_back((*itr));
        }

        out->writeBool(false);
        out->writeUIntArray(strarr.get());
    }
}

void Text::read(DataInputStream* in){
    // Peek on Text's identification.
    int id = in->peekInt();
    if(id == IVETEXT){
        // Read Text's identification.
        id = in->readInt();
        // If the osg class is inherited by any other class we should also read this from file.
        osg::Object*  obj = dynamic_cast<osg::Object*>(this);
        if(obj){
            ((ive::Drawable*)(obj))->read(in);
        }
        else
            in_THROW_EXCEPTION("Text::read(): Could not cast this osgText::Text to an osg::Drawable.");
        // Read Text's properties

    unsigned int width, height;
    float c_height, aspectRatio;

    setFont(in->readString());

    width = in->readUInt();
    height = in->readUInt();

    setFontResolution(width,height);

    c_height = in->readFloat();
    aspectRatio = in->readFloat();

    setCharacterSize(c_height,aspectRatio);

    setCharacterSizeMode((osgText::TextBase::CharacterSizeMode) in->readUInt());

    setMaximumWidth(in->readFloat());
    setMaximumHeight(in->readFloat());

    if ( in->getVersion() >= VERSION_0020 )
    {
        setLineSpacing(in->readFloat());
    }

    setAlignment((osgText::TextBase::AlignmentType) in->readUInt());

    //Nothing to do...
    //setAxisAlignment((osgText::Text::AxisAlignment) in->readUint());

    setRotation(in->readQuat());
    setAutoRotateToScreen(in->readBool());
    setLayout((osgText::Text::Layout) in->readUInt());

    setPosition(in->readVec3());
    setColor(in->readVec4());
    setDrawMode(in->readUInt());

    if ( in->getVersion() >= VERSION_0041 )
    {
        setBoundingBoxMargin(in->readFloat());
        setBoundingBoxColor(in->readVec4());
    }

    if ( in->getVersion() >= VERSION_0028 )
    {
        setBackdropType((osgText::Text::BackdropType) in->readUInt());

        float horizontalOffset,verticalOffset;
        horizontalOffset = in->readFloat();
        verticalOffset = in->readFloat();
        setBackdropOffset(horizontalOffset,verticalOffset);

        setBackdropColor(in->readVec4());
        setBackdropImplementation((osgText::Text::BackdropImplementation) in->readUInt());
        setColorGradientMode((osgText::Text::ColorGradientMode) in->readUInt());

        osg::Vec4 colorGradientTopLeft,colorGradientBottomLeft,colorGradientBottomRight,colorGradientTopRight; 
        colorGradientTopLeft = in->readVec4();
        colorGradientBottomLeft = in->readVec4();
        colorGradientBottomRight = in->readVec4();
        colorGradientTopRight = in->readVec4();
        setColorGradientCorners(colorGradientTopLeft,colorGradientBottomLeft,colorGradientBottomRight,colorGradientTopRight);
    }

    if(in->readBool())
        setText(in->readString());
    else
    {
        if ( in->getVersion() >= VERSION_0018 )
        {
            osgText::String textstr;
            osg::ref_ptr<osg::UIntArray> arr = in->readUIntArray();
            for(unsigned int i = 0; i < arr->getNumElements(); i++)
            {
                textstr.push_back( arr->at(i) );
            }

            setText(textstr);
        }
        else
        {
            // buggy original path, should have used a UIntArray originally, now fixed above.
            std::string textstr;
            osg::ref_ptr<osg::UByteArray> arr = in->readUByteArray();
            for(unsigned int i = 0; i < arr->getNumElements(); i++)
            {
                textstr += (char) arr->at(i);
            }

            setText(textstr);
        }
    }

    }
    else{
        in_THROW_EXCEPTION("Text::read(): Expected Text identification.");
    }
}
