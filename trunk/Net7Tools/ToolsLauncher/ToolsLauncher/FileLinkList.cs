using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ToolsLauncher
{
    class FileLinkList
    {
        public String ExeFile;
        public Button BObject;

        public FileLinkList(String ExeF, Button Object)
        {
            ExeFile = ExeF;
            BObject = Object;
        }
    }
}
