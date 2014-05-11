using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MissionEditor.Nodes;

namespace MissionEditor.Gui
{
    public partial class DlgStages : Form
    {
        private Boolean m_madeSelection;
        private Stage m_stage;

        public DlgStages()
        {
            InitializeComponent();
            m_stage = null;
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);
            guiDescriptionTxt.Text = "";
            m_madeSelection = false;
            guiDescriptionTxt.Focus();
        }

        public void setId(int id)
        {
            guiIdTxt.Text = id.ToString();
        }

        public void editStage(Stage stage)
        {
            m_stage = stage;
            //TODO: Display the Stage when not null
        }

        /// <summary>
        /// Validate the entered data and close the dialog
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void onOk(object sender, EventArgs e)
        {
            m_stage = new Stage();
            m_stage.setId(guiIdTxt.Text);
            m_stage.setDescription(guiDescriptionTxt.Text);

            String error;
            m_stage.addValidations(Stage.ValidationType.FromDialog);
            if (Database.DataConfiguration.validate(out error))
            {
                m_madeSelection = true;
                this.Close();
            }
            else
            {
                MessageBox.Show(error);
            }
        }

        /// <summary>
        /// Close the dialog
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void onCancel(object sender, EventArgs e)
        {
            this.Close();
        }

        public Boolean getValues(out Stage stage)
        {
            stage = m_stage;
            return m_madeSelection;
        }

    }
}
