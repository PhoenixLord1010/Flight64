#ifndef __MGL_CALLBACK_H__
#define __MGL_CALLBACK_H__
/**
 * mgl_callback
 * @license The MIT License (MIT)
   @copyright Copyright (c) 2015 EngineerOfLies
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

/**
 * @brief Callbacks are data, function pointer pairs to be used whenever a callback is appropriate
 *
 */
typedef struct
{
    void (*function)(void *data,void *context); /**<a pointer to the function to call, data is the data 
                                                    provided in callback, context is optional data provdied 
                                                    by caller and needs to be expected by the callback or ignored*/
    void *data;                                 /**<a pointer to the data that will be passed to the funciton*/
}MglCallback;

/**
 * @brief helper function to create a callback pair
 * @param function the function to set
 * @param data the data to set
 */
MglCallback mgl_callback(void (*function)(void *data,void *context),void *data);

/**
 * @brief copy one callback into another
 * @param dst the callback to be set
 * @param src the callback to be copied
 */
void mgl_callback_copy(MglCallback *dst,MglCallback src);

/**
 * @brief set the values of an existing callback
 * @param cb the callback to set
 * @param function the function to set
 * @param data the data to set
 */
void mgl_callback_set(MglCallback *cb,void (*function)(void *data,void *context),void *data);

/**
 * @brief allocate and zero a new callback pair
 * @return NULL on error, or newly blank callback pair
 */
MglCallback *mgl_callback_new();

/**
 * @brief frees a callback and sets your pointer to it to NULL
 * @param a pointer to a callback pointer
 */
void mgl_callback_free(MglCallback **cb);

/**
 * @brief frees a callback
 * @param a pointer to a callback
 */
void mgl_callback_delete(MglCallback *cb);


#endif
