#ifndef _FORMAT_BITSTREAM_H
#define _FORMAT_BITSTREAM_H
/*********************************************************************
  Copyright (c) 1995 ISO/IEC JTC1 SC29 WG1, All Rights Reserved
  formatBitstream.h
**********************************************************************/

/*
  Revision History:

  Date        Programmer                Comment
  ==========  ========================= ===============================
  1995/09/06  mc@fivebats.com           created

*/
#ifndef MAX_CHANNELS
#define MAX_CHANNELS 2
#endif

#ifndef MAX_GRANULES
#define MAX_GRANULES 2
#endif

void L3_formatbits_initialise(shine_global_config *config);
void L3_formatbits_close(shine_global_config *config);

/*
  The following is a shorthand bitstream syntax for
  the type of bitstream this package will create.
  The bitstream has headers and side information that
  are placed at appropriate sections to allow framing.
  The main data is placed where it fits in a manner
  similar to layer3, which means that main data for a
  frame may be written to the bitstream before the
  frame's header and side information is written.

BitstreamFrame()
{
    Header();
    FrameSI();

    for ( ch )
        ChannelSI();

    for ( gr )
        for ( ch )
            SpectrumSI();

    MainData();
}

MainData()
{
    for ( gr )
        for ( ch )
        {
            Scalefactors();
            CodedData();
            UserSpectrum();
        }
    UserFrameData();
}

*/

/*
  public functions in formatBitstream.c
*/

/* count the bits in a BitstreamPart */
int  BF_PartLength( BF_BitstreamPart *part );

/* encode a frame of audio and write it to your bitstream */
void BF_BitstreamFrame( shine_global_config *config);

BF_PartHolder *BF_newPartHolder( unsigned long int max_elements );
BF_PartHolder *BF_resizePartHolder( BF_PartHolder *oldPH, int max_elements );
BF_PartHolder *BF_addElement( BF_PartHolder *thePH, BF_BitstreamElement *theElement );
BF_PartHolder *BF_addEntry( BF_PartHolder *thePH, unsigned long int value, unsigned int length );
BF_PartHolder *BF_NewHolderFromBitstreamPart( BF_BitstreamPart *thePart );
BF_PartHolder *BF_LoadHolderFromBitstreamPart( BF_PartHolder *theHolder, BF_BitstreamPart *thePart );
BF_PartHolder *BF_freePartHolder( BF_PartHolder *thePH );
#endif
