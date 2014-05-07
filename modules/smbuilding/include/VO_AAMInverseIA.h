/****************************************************************************************************
*                                                                                                   *
*                   IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.               *
*                                                                                                   *
*   By downloading, copying, installing or using the software you agree to this license.            *
*   If you do not agree to this license, do not download, install, copy or use the software.        *
*                                                                                                   *
*                                   License Agreement                                               *
*                           For Vision Open Statistical Models                                      *
*                                                                                                   *
* Copyright (C):    2006~2014 by JIA Pei, all rights reserved.                                      *
*                                                                                                   *
*                   VOSM is free software under the terms of the GNU Lesser General Public License  *
*                   (GNU LGPL) as published by the Free Software Foundation; either version 3.0 of  *
*                   the License, or (at your option) any later version.                             *
*                   You can use it, modify it, redistribute it, etc; and redistribution and use in  *
*                   source and binary forms, with or without modification, are permitted provided   *
*                   that the following conditions are met:                                          *
*                                                                                                   *
*                   a) Redistribution's of source code must retain this whole paragraph of          *
*                   copyright notice, including this list of conditions and all the following       *
*                   contents in this copyright paragraph.                                           *
*                                                                                                   *
*                   b) Redistribution's in binary form must reproduce this whole paragraph of       *
*                   copyright notice, including this list of conditions and all the following       *
*                   contents in this copyright paragraph, and/or other materials provided with      *
*                   the distribution.                                                               *
*                                                                                                   *
*                   c) The name of the copyright holders may not be used to endorse or promote      *
*                   products derived from this software without specific prior written permission.  *
*                                                                                                   *
*                   Any publications based on this code must cite the following five papers,        *
*                   technic reports and on-line materials.                                          *
*                                                                                                   *
*                   1) P. JIA, 2D Statistical Models, Technical Report of Vision Open Working       *
*                   Group, 2st Edition, Oct 21, 2010.                                               *
*                   http://www.visionopen.com/members/jiapei/publications/pei_sm2dreport2010.pdf    *
*                                                                                                   *
*                   2) P. JIA. Audio-visual based HMI for an Intelligent Wheelchair.                *
*                   PhD thesis, University of Essex, 2010.                                          *
*                   http://www.visionopen.com/members/jiapei/publications/pei_thesischapter34.pdf   *
*                                                                                                   *
*                   3) T. Cootes and C. Taylor. Statistical models of appearance for computer       *
*                   vision. Technical report, Imaging Science and Biomedical Engineering,           *
*                   University of Manchester, March 8 2004.                                         *
*                                                                                                   *
*                   4) I. Matthews and S. Baker. Active appearance models revisited.                *
*                   International Journal of Computer Vision, 60(2):135–164, November 2004.         *
*                                                                                                   *
*                   5) M. B. Stegmann, Active Appearance Models: Theory, Extensions and Cases,      *
*                   http://www2.imm.dtu.dk/~aam/main/, 2000.                                        *
*                                                                                                   *
*                                                                                                   *
* Version:          0.3.4                                                                           *
* Author:           JIA Pei                                                                         *
* Contact:          jp4work@gmail.com                                                               *
* URL:              http://www.visionopen.com                                                       *
* Create Date:      2010-11-04                                                                      *
* Modify Date:      2014-04-17                                                                      *
****************************************************************************************************/


#ifndef __VO_AAMINVERSEIA__
#define __VO_AAMINVERSEIA__


#include <vector>

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "VO_AXM.h"



/** 
 * @author		JIA Pei
 * @brief		Inverse image alignment Model.
 */
class VO_AAMInverseIA : public VO_AXM
{
friend class VO_Fitting2DSM;
friend class VO_FittingAAMBasic;
friend class VO_FittingAAMForwardIA;
friend class VO_FittingAAMInverseIA;
friend class VO_FittingASMLTCs;
friend class VO_FittingASMNDProfiles;
friend class VO_FittingAFM;
private:
    cv::Mat                   	m_IplImageTempFaceX;
    cv::Mat                   	m_IplImageTempFaceY;
    cv::Mat                   	m_IplImageTempFace;
protected:
    /** "Revisited" P26-27, 4*116 */
    cv::Mat_<float>             m_MatSimilarityTransform;

    /** Steepest Descent Images for each point, 90396*20 */
    cv::Mat_<float>				m_MatSteepestDescentImages4ShapeModel;

    /** Steepest Descent Images for global shape normalization, 90396*4 */
    cv::Mat_<float>				m_MatSteepestDescentImages4GlobalShapeNormalization;

    /** Combined Steepest Descent Images, 90396*24 */
    cv::Mat_<float>				m_MatSteepestDescentImages;

    /** Combined Modified Steepest Descent Images, 90396*24 */
    cv::Mat_<float>				m_MatModifiedSteepestDescentImages;

    /** Hessian Matrix, actually, Hessian matrix is summed up from all the pixels in the image, 20*20, or 24*24 */
    cv::Mat_<float>				m_MatHessianMatrixInverse;

    /** Pre computed matrix 24*90396 */
    cv::Mat_<float>             m_MatICIAPreMatrix;

    /** Initialization */
    void                        init();

public:
    /** Default constructor to create a VO_AAMInverseIA object */
    VO_AAMInverseIA();

    /** Destructor */
    virtual ~VO_AAMInverseIA();

    /** Pre-computation for Inverse Compositional Image Alignment AAM Fitting */

    /** Calculate gradients in both X and Y directions for template face */
    void						VO_CalcTemplateFaceGradients();

    /** Calculate steepest descent image for template face */
    void						VO_CalcSDI();

    /** Calculate modified steepest descent image for template face */
    void						VO_CalcModifiedSDI();

    /** Calculate inverse Hessian matrix for template face */
    void						VO_CalcInverseHessian();

    /** Calculate Hessian matrix * MSDI^T */
    void						VO_CalcICIAPreMatrix();

    /** Build ICIA AAM model */
    void						VO_BuildAAMICIA(const std::vector<std::string>& allLandmarkFiles4Training,
												const std::vector<std::string>& allImgFiles4Training,
												const std::string& shapeinfoFileName, 
												unsigned int database,
												unsigned int channels = 3,
												unsigned int levels = 1,
												int trm = VO_Features::DIRECT, 
												float TPShape = 0.95f, 
												float TPTexture = 0.95f, 
												bool useKnownTriangles = false);

    /** Save AAMICIA, to a specified folder */
    void                        VO_Save(const std::string& fd);

    /** Load all parameters */
    void                        VO_Load(const std::string& fd);

    /** Load Parameters for fitting */
    void                        VO_LoadParameters4Fitting(const std::string& fd);
};

#endif // __VO_AAMINVERSEIA__

