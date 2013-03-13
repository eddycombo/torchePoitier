/*
 *  ofxCvContourSimplify.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 18/09/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

class ofxCvContourSimplify
{

public:

	//-- douglas pecker approximation using openCV

	void simplify ( const vector<ofPoint>& contourIn, vector<ofPoint>& contourOut, float tolerance )
	{
		//-- copy points.

		int numOfPoints;
		numOfPoints = contourIn.size();
//		numOfPoints += ( contourIn.size() > 0 ) ? 1 : 0;

		CvPoint* cvpoints;
		cvpoints = new CvPoint[ numOfPoints ];

		for( int i=0; i<numOfPoints; i++)
		{
			int j = i % numOfPoints;

			cvpoints[ i ].x = contourIn[ j ].x;
			cvpoints[ i ].y = contourIn[ j ].y;
		}

		//-- create contour.

		CvContour	contour;
		CvSeqBlock	contour_block;

		cvMakeSeqHeaderForArray
		(
			CV_SEQ_POLYLINE,	// CV_SEQ_POLYLINE, CV_SEQ_POLYGON, CV_SEQ_CONTOUR, CV_SEQ_SIMPLE_POLYGON
			sizeof(CvContour),
			sizeof(CvPoint),
			cvpoints,
			numOfPoints,
			(CvSeq*)&contour,
			&contour_block
		);

		//-- simplify contour.

		CvMemStorage* storage;
		storage = cvCreateMemStorage( 1000 );

		CvSeq *result = 0;
		result = cvApproxPoly
		(
			&contour,
			sizeof( CvContour ),
			storage,
			CV_POLY_APPROX_DP,
			cvContourPerimeter( &contour ) * tolerance,
			0
		);

		//-- contour out points.

		contourOut.clear();
		for( int j=0; j<result->total; j++ )
		{
			CvPoint * pt = (CvPoint*)cvGetSeqElem( result, j );

			contourOut.push_back( ofPoint() );
			contourOut.back().x = (float)pt->x;
			contourOut.back().y = (float)pt->y;
		}

		//-- clean up.

		if( storage != NULL )
			cvReleaseMemStorage( &storage );

		delete[] cvpoints;
	}
    void smooth ( vector<ofPoint>& contourIn, vector<ofPoint>& contourOut, float smoothPct, float smoothWrap )
{
	//contourOut.clear();
	//contourOut.assign( contourIn.size(), contourIn );

	smoothPct = 1 - MIN( MAX( smoothPct, 0 ), 1 );

	float invPct;
	invPct = 1.0 - smoothPct;

	for( int i=0; i<contourOut.size(); i++ )			// first copy the data
	{
		contourOut[ i ] = contourIn[ i ];
	}

	smoothWrap = MAX( smoothWrap, 0.0 );	// make sure its not less then zero.

	int t = contourOut.size();		// total.
	int w = t + t * smoothWrap;		// wrap.
	for( int i=1; i<w; i++ )
	{
		int j = ( i - 0 ) % t;		// current position.
		int k = ( i - 1 ) % t;		// previous position.

		contourOut[ j ].x = contourOut[ j ].x * smoothPct + contourOut[ k ].x * invPct;
		contourOut[ j ].y = contourOut[ j ].y * smoothPct + contourOut[ k ].y * invPct;
	}
}
};
