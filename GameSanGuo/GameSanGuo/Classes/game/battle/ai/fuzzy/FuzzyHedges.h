#ifndef FUZZY_HEDGES_H
#define FUZZY_HEDGES_H
//-----------------------------------------------------------------------------
//
//  Name:   FuzzyHedges.h
//
//  Author: angel
//
//  Desc:   classes to implement fuzzy hedges 
//-----------------------------------------------------------------------------
#include "FuzzySet.h"
#include "FuzzyTerm.h"
#include <math.h>

class FzVery : public FuzzyTerm
{
public:

  FuzzySet& m_Set;

  //
  FzVery(const FzVery& inst):m_Set(inst.m_Set){}
  FzVery& operator=(const FzVery&);
 

public:

  FzVery(FzSet& ft):m_Set(ft.m_Set){}

  double GetDOM()const
  {
    return m_Set.GetDOM() * m_Set.GetDOM();
  }

  FuzzyTerm* Clone()const{return new FzVery(*this);}

  void ClearDOM(){m_Set.ClearDOM();}
  void ORwithDOM(double val){m_Set.ORwithDOM(val * val);}
};

///////////////////////////////////////////////////////////////////////////////
class FzFairly : public FuzzyTerm
{
public:

  FuzzySet& m_Set;

  //
  FzFairly(const FzFairly& inst):m_Set(inst.m_Set){}
  FzFairly& operator=(const FzFairly&);

public:

  FzFairly(FzSet& ft):m_Set(ft.m_Set){}

  double GetDOM()const
  {
    return sqrt(m_Set.GetDOM());
  }

  FuzzyTerm* Clone()const{return new FzFairly(*this);}

  void ClearDOM(){m_Set.ClearDOM();}
  void ORwithDOM(double val){m_Set.ORwithDOM(sqrt(val));}
};



#endif