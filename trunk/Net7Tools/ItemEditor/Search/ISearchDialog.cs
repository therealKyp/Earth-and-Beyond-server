using System;
using System.Collections.Generic;
using System.Text;

namespace Net7_Tools.Search
{
    public interface ISearchDialog
    {
        String getSelectedItemId();
        void setSelectedItemId(String selectedItemId);
        System.Windows.Forms.Form getForm();
    }
}
