//*************************************************************************************************
//
//                            #####             #####             #####
//                          #########         #########         #########
//                         ##                ##       ##       ##
//                        ##                ##         ##        #####
//                        ##     #####      ##         ##           #####
//                         ##       ##       ##       ##                ##
//                          #########         #########         #########
//                            #####             #####             #####
//
//                                      (c) Ahmed Gazar, #year
//
//*************************************************************************************************
//! @file       #source_name
//! @author     #author
//! @date       #date
//! @version    1.0
//!
//! @brief      TODO: brief description.
//! @details    For a more detailed description of this service, please refer to @ref #header_name
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        #date    #author    Initial version created
//*************************************************************************************************
//
// Copyright (c) #year Ahmed Gazar
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//*************************************************************************************************
/*
 * Includes
 */
#includes

/*
 * Macros
 */
#macros

/*
 * Static variables
 */
#static_variables

/*
 * Function prototypes
 */
#function_prototypes
#taskdescriptors

/*
 * Function: #initfunction_name
 */
gos_result_t #initfunction_name (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t #initresult = GOS_SUCCESS;

    /*
     * Function code.
     */
    if (gos_taskRegister(&#taskdescriptorname, &#taskidname) != GOS_SUCCESS)
    {
        #initresult = GOS_ERROR;
    }
    else
    {
        // Nothing to do.
    }

    return #initresult;
}

/**
 * @brief   TODO
 * @details TODO
 *
 * @return    -
 */
GOS_STATIC void_t #taskfunction (void_t)
{
    #taskfunction_body
}
