using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SQLBind
{
    class EffectComboHandel
    {
        public TextBox Var1, Var2, Var3;
        public Label Var1Lable, Var2Lable, Var3Lable;
        public Label EffectString;
        public ComboBox EffectBox;

        public void EnableFields(bool Enable)
        {
            EffectBox.Enabled = Enable;
            Var1.Enabled = Enable;
            Var2.Enabled = Enable;
            Var3.Enabled = Enable;
        }
    }
}
