#ifndef FUZZYSET_RIGHTSHOULDER_H
#define FUZZYSET_RIGHTSHOULDER_H
//-----------------------------------------------------------------------------
//
//  Name:   FuzzySet_RightShoulder.h
//
//  Author: angel
//
//  Desc:   definition of a fuzzy set that has a right shoulder shape. (the
//          maximum value this variable can accept is *any* value greater than 
//          the midpoint.
//-----------------------------------------------------------------------------
#include "FuzzySet.h"
#include "misc/utils.h"



class FuzzySet_RightShoulder : public FuzzySet
{
private:

  
  double   m_dPeakPoint;
  double   m_dLeftOffset;
  double   m_dRightOffset;

public:
  
  FuzzySet_RightShoulder(double peak,
                         double LeftOffset,
                         double RightOffset):
  
                  FuzzySet( ((peak + RightOffset) + peak) / 2),
                  m_dPeakPoint(peak),
                  m_dLeftOffset(LeftOffset),
                  m_dRightOffset(RightOffset)

  {}

  
  double CalculateDOM(double val)const;
};


#endif