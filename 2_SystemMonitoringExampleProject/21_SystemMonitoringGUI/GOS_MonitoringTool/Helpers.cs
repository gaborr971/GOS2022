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
//                                      (c) Ahmed Gazar, 2023
//
//*************************************************************************************************
//! @file       Helpers.cs
//! @author     Ahmed Gazar
//! @date       2023-09-22
//! @version    1.0
//!
//! @brief      Helper methods.
//! @details    Helper methods.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2023-09-22    Ahmed Gazar     Initial version created
//*************************************************************************************************
//
// Copyright (c) 2023 Ahmed Gazar
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
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace GOS_MonitoringTool
{
    /// <summary>
    /// Helper class.
    /// </summary>
    public class Helpers
    {
        private delegate void SetLabelTextDelegate(Label label, string Text);

        /// <summary>
        /// Thread-safe wrapper for label text setting.
        /// </summary>
        /// <param name="control"></param>
        /// <param name="label"></param>
        /// <param name="text"></param>
        public static void SetLabelText_ThreadSafe(Control control, Label label, string text)
        {
            try
            {
                if (label.InvokeRequired)
                {
                    SetLabelTextDelegate d = new SetLabelTextDelegate(SetLabelText);
                    control.Invoke(d, label, text);
                }
                else
                {
                    SetLabelText(label, text);
                }
            }
            catch
            {

            }
        }

        /// <summary>
        /// Label text setting method.
        /// </summary>
        /// <param name="label"></param>
        /// <param name="text"></param>
        private static void SetLabelText(Label label, string text)
        {
            label.Text = text;
        }

        private delegate void UpdateListViewWithItemDelegate(ListView listview, List<ListViewItem> listViewItems);

        /// <summary>
        /// Thread-safe wrapper for listview updating.
        /// </summary>
        /// <param name="control"></param>
        /// <param name="listView"></param>
        /// <param name="listViewItems"></param>
        public static void UpdateListViewWithItems_ThreadSafe (Control control, ListView listView, List<ListViewItem> listViewItems)
        {
            try
            {
                if (listView.InvokeRequired)
                {
                    UpdateListViewWithItemDelegate d = new UpdateListViewWithItemDelegate(UpdateListViewWithItems);
                    control.Invoke(d, listView, listViewItems);
                }
                else
                {
                    UpdateListViewWithItems(listView, listViewItems);
                }
            }
            catch
            {

            }
        }

        /// <summary>
        /// Listview updating method.
        /// </summary>
        /// <param name="listView"></param>
        /// <param name="listViewItems"></param>
        private static void UpdateListViewWithItems(ListView listView, List<ListViewItem> listViewItems)
        {
            listView.Items.Clear();
            listView.Items.AddRange(listViewItems.ToArray());
        }

        private delegate void ResizeListViewDelegate(ListView listView);

        /// <summary>
        /// Thread-safe wrapper for listview resizing.
        /// </summary>
        /// <param name="control"></param>
        /// <param name="listView"></param>
        public static void ResizeListView_ThreadSafe (Control control, ListView listView)
        {
            try
            {
                if (listView.InvokeRequired)
                {
                    ResizeListViewDelegate d = new ResizeListViewDelegate(ResizeListView);
                    control.Invoke(d, listView);
                }
                else
                {
                    ResizeListView(listView);
                }
            }
            catch
            {

            }
        }

        /// <summary>
        /// Listview resizing method.
        /// </summary>
        /// <param name="listView"></param>
        private static void ResizeListView(ListView listView)
        {
            listView.AutoResizeColumn(1, ColumnHeaderAutoResizeStyle.ColumnContent);
            listView.AutoResizeColumn(2, ColumnHeaderAutoResizeStyle.HeaderSize);
            listView.AutoResizeColumn(3, ColumnHeaderAutoResizeStyle.HeaderSize);
            listView.AutoResizeColumn(4, ColumnHeaderAutoResizeStyle.HeaderSize);
            listView.AutoResizeColumn(5, ColumnHeaderAutoResizeStyle.HeaderSize);
            listView.AutoResizeColumn(6, ColumnHeaderAutoResizeStyle.HeaderSize);
            listView.AutoResizeColumn(7, ColumnHeaderAutoResizeStyle.HeaderSize);
            listView.AutoResizeColumn(8, ColumnHeaderAutoResizeStyle.ColumnContent);
            listView.AutoResizeColumn(9, ColumnHeaderAutoResizeStyle.ColumnContent);
            listView.AutoResizeColumn(10, ColumnHeaderAutoResizeStyle.ColumnContent);
        }

        private delegate void SetCheckBoxParametersDelegate(CheckBox checkBox, string text, Color backgroundColor, bool check);

        /// <summary>
        /// Thread-safe wrapper for checkbox parameter setting.
        /// </summary>
        /// <param name="control"></param>
        /// <param name="checkBox"></param>
        /// <param name="text"></param>
        /// <param name="backgroundColor"></param>
        /// <param name="check"></param>
        public static void SetCheckBoxParameters_ThreadSafe (Control control, CheckBox checkBox, string text, Color backgroundColor, bool check)
        {
            try
            {
                if (checkBox.InvokeRequired)
                {
                    SetCheckBoxParametersDelegate d = new SetCheckBoxParametersDelegate(SetCheckBoxParameters);
                    control.Invoke(d, checkBox, text, backgroundColor, check);
                }
                else
                {
                    SetCheckBoxParameters(checkBox, text, backgroundColor, check);
                }
            }
            catch
            {

            }
        }

        /// <summary>
        /// Checkbox parameter setter method.
        /// </summary>
        /// <param name="checkBox"></param>
        /// <param name="text"></param>
        /// <param name="backgroundColor"></param>
        /// <param name="check"></param>
        private static void SetCheckBoxParameters(CheckBox checkBox, string text, Color backgroundColor, bool check)
        {
            checkBox.Text = text;
            checkBox.BackColor = backgroundColor;
            checkBox.Checked = check;
        }
    }
}
