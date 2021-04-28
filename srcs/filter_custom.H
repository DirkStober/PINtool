/*
 * Copyright 2002-2020 Intel Corporation.
 * 
 * This software is provided to you as Sample Source Code as defined in the accompanying
 * End User License Agreement for the Intel(R) Software Development Products ("Agreement")
 * section 1.L.
 * 
 * This software and the related documents are provided as is, with no express or implied
 * warranties, other than those that are expressly stated in the License.
 */

#ifndef FILTER_C_H
#define FILTER_C_H
#include <stdio.h>
//#define EXCLUDE_OMP

using std::string;
namespace NDP_FILTER
{

class FILTER_RTN
{
  public:
    FILTER_RTN(const string& prefix="", const string& knob_family="pintool") : 
        _rtnsKnob(KNOB_MODE_APPEND, knob_family, prefix+"filter_rtn", "", 
                  "Routines to instrument") 
    {}
    
    /*! @ingroup FILTER_RTN
      Activate the filter. Must be done before PIN_StartProgram
    */
    VOID Activate()
    {
        PIN_InitSymbols();
        _activated = true;
    }
    
    /*! @ingroup FILTER_RTN
      Return true if the filter is not active or the routine that contains this trace is selected
    */
    BOOL SelectTrace(TRACE trace)
    {
        ASSERTX(_activated);
        
        if (!RTN_Valid(TRACE_Rtn(trace)))
        {
            if (_rtnsKnob.NumberOfValues() > 0)
                return false;
            else
                return true;
        }
        
        return SelectRtn(TRACE_Rtn(trace));
    }

    /*! @ingroup FILTER_RTN
      Return true if the filter is not active or the routine is selected
    */
    BOOL SelectRtn(RTN rtn)
    {
        ASSERTX(RTN_Valid(rtn));
        ASSERTX(_activated);

        UINT32 numRtns = _rtnsKnob.NumberOfValues();
        
        // No rtn based selection
        if (numRtns == 0)
            return true;

        // RTN must be on list for selection
        for (UINT32 i = 0; i < numRtns; i++)
        {
            if (RTN_Name(rtn) == _rtnsKnob.Value(i))
                return true;
	    // Get the undecorated Name
	    string rtn_name = PIN_UndecorateSymbolName(RTN_Name(rtn),UNDECORATION_NAME_ONLY);
            if (rtn_name == _rtnsKnob.Value(i)){
                return true;
	    }
#ifdef EXCLUDE_OMP
	    // For openmp compare first four chars
	    if ((rtn_name.compare(0,4,"gomp") && rtn_name.compare(0,4,"GOMP")) == 0){
		    return true;
	    }
#endif
        }

        return false;
    }
 
  private:
    BOOL _activated;
    KNOB<string> _rtnsKnob;

};

/*! @defgroup FILTER_LIB
  @ingroup FILTER
  Filter for selecting shared libraries
  Use -filter_no_shared_libs to ignore all shared libraries
*/

/*! @ingroup FILTER_LIB
*/
class FILTER_LIB
{
  public:
    FILTER_LIB(const string& prefix="", 
               const string& knob_family="pintool") 
        : _noSharedLibKnob(KNOB_MODE_WRITEONCE, knob_family, 
                           prefix+"filter_no_shared_libs", "", 
                           "Do not instrument shared libraries") 
    {}

    /*! @ingroup FILTER_LIB
      Activate the filter. Must be done before PIN_StartProgram
    */
    VOID Activate()
    {}

    /*! @ingroup FILTER_LIB
      Return true if the filter is not active or the shared library that contains this trace is selected
    */
    BOOL SelectTrace(TRACE trace)
    {
        if (_noSharedLibKnob.Value()
            && (!RTN_Valid(TRACE_Rtn(trace))
                || !IMG_Valid(SEC_Img(RTN_Sec(TRACE_Rtn(trace))))
                || !IMG_IsMainExecutable(SEC_Img(RTN_Sec(TRACE_Rtn(trace)))) ))
            return false;

        return true;
    }

  private:
    KNOB<BOOL> _noSharedLibKnob;
};


/*! @defgroup FILTER_MULTI
  @ingroup FILTER

  Filter that includes all the filters
  See @ref FILTER_RTN, @ref FILTER_LIB
*/

/*! @ingroup FILTER_MULTI
*/
class FILTER
{
  public:

    FILTER(const string& prefix="",
           const string& knob_family="pintool") :
        _filterRtn(prefix, knob_family),
        _filterLib(prefix, knob_family)
    {}

    /*! @ingroup FILTER_MULTI
      Activate the filter. Must be done before PIN_StartProgram
    */
    VOID Activate()
    {
        _filterRtn.Activate();
        _filterLib.Activate();
    }

    /*! @ingroup FILTER_MULTI
      Return true if the filter is not active or the this trace is selected
    */
    BOOL SelectTrace(TRACE trace)
    {
        if (!_filterRtn.SelectTrace(trace)
            || !_filterLib.SelectTrace(trace)
        )
            return false;

        return true;
    }
    
  private:
    FILTER_RTN _filterRtn;
    FILTER_LIB _filterLib;
};

}
#endif
