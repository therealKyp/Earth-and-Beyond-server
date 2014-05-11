using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using MissionEditor.Nodes;
using CommonTools;
using CommonTools.Database;
using MissionEditor.Database;

namespace MissionEditor.Gui
{
    public partial class DlgCompletions : Form
    {
        private Boolean m_madeSelection;
        Completion m_completion;
        Database.CodeDescSearch[] m_codeSearch;

        public DlgCompletions()
        {
            InitializeComponent();

            m_completion = null;
            m_codeSearch = new Database.CodeDescSearch[2];
            m_codeSearch[0] = new CodeDescSearch(guiValueTxt, guiValueDescriptionTxt, guiValueSearchBtn);
            m_codeSearch[1] = new CodeDescSearch(guiDataTxt, guiDataDescriptionTxt, guiDataSearchBtn);
            guiTypeCbo.Items.Clear();
            CommonTools.Enumeration.AddSortedByName<CompletionType>(guiTypeCbo);
            guiTypeCbo.SelectedIndex = 0;
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);
            m_madeSelection = false;
            guiTypeCbo.Focus();
        }

        public void editCompletion(Completion completion)
        {
            m_completion = completion;
            // TODO: Display Completion for edit
            guiTypeCbo.SelectedItem = m_completion.getCompletionType();
            // Ensure fields are properly enabled
            // Display other values
        }

        private void onTypeSelected(object sender, EventArgs e)
        {
            guiValueTxt.Enabled = false;
            guiValueSearchBtn.Enabled = false;
            guiDataTxt.Enabled = false;
            guiDataSearchBtn.Enabled = false;
            guiAmountTxt.Enabled = false;

            CompletionType completionType = (CompletionType)guiTypeCbo.SelectedItem;
            switch (completionType)
            {
                case CompletionType.Arrive_At:
                    guiValueTxt.Enabled = true;
                    guiValueSearchBtn.Enabled = true;
                    m_codeSearch[0].setDataType(DataConfiguration.DataType.sector_object);
                    break;
                case CompletionType.Nav_Message:
                    guiValueTxt.Enabled = true;
                    guiValueSearchBtn.Enabled = true;
                    m_codeSearch[0].setDataType(DataConfiguration.DataType.sector_object);
                    guiAmountTxt.Enabled = true;
                    break;
                case CompletionType.Proximity_To_Space_Npc:
                    guiValueTxt.Enabled = true;
                    guiValueSearchBtn.Enabled = true;
                    m_codeSearch[0].setDataType(DataConfiguration.DataType.sector_object);
                    break;
                case CompletionType.Talk_Space_Npc:
                    guiValueTxt.Enabled = true;
                    guiValueSearchBtn.Enabled = true;
                    m_codeSearch[0].setDataType(DataConfiguration.DataType.sector_object);
                    break;
                case CompletionType.Fight_Mob:
                    guiValueTxt.Enabled = true;
                    guiValueSearchBtn.Enabled = true;
                    m_codeSearch[0].setDataType(DataConfiguration.DataType.mob);
                    guiAmountTxt.Enabled = true;
                    break;
                case CompletionType.Give_Credits:
                    guiAmountTxt.Enabled = true;
                    break;
                case CompletionType.Give_Item:
                    guiValueTxt.Enabled = true;
                    guiValueSearchBtn.Enabled = true;
                    m_codeSearch[0].setDataType(DataConfiguration.DataType.item);
                    guiAmountTxt.Enabled = true;
                    break;
                case CompletionType.Nearest_Nav:
                    guiValueTxt.Enabled = true;
                    guiValueSearchBtn.Enabled = true;
                    m_codeSearch[0].setDataType(DataConfiguration.DataType.sector_object);
                    break;
                case CompletionType.Obtain_Items:
                    guiValueTxt.Enabled = true;
                    guiValueSearchBtn.Enabled = true;
                    m_codeSearch[0].setDataType(DataConfiguration.DataType.item);
                    break;
                /*case CompletionType.Obtain_Items_At_Location:
                    guiValueTxt.Enabled = true;
                    guiValueSearchBtn.Enabled = true;
                    m_codeSearch[0].setDataType(DataConfiguration.DataType.sector_object);
                    guiDataTxt.Enabled = true;
                    guiDataSearchBtn.Enabled = true;
                    m_codeSearch[1].setDataType(DataConfiguration.DataType.item);
                    break;*/
                case CompletionType.Possess_Item:
                    guiValueTxt.Enabled = true;
                    guiValueSearchBtn.Enabled = true;
                    m_codeSearch[0].setDataType(DataConfiguration.DataType.item);
                    guiAmountTxt.Enabled = true;
                    break;
                case CompletionType.Receive_Item:
                    guiAmountTxt.Enabled = true;
                    break;
                case CompletionType.Current_Sector:
                    guiValueTxt.Enabled = true;
                    guiValueSearchBtn.Enabled = true;
                    m_codeSearch[0].setDataType(DataConfiguration.DataType.sector);
                    break;
                /*case CompletionType.Take_Item_To_Location:
                    guiValueTxt.Enabled = true;
                    guiValueSearchBtn.Enabled = true;
                    m_codeSearch[0].setDataType(DataConfiguration.DataType.sector_object);
                    guiDataTxt.Enabled = true;
                    guiDataSearchBtn.Enabled = true;
                    m_codeSearch[1].setDataType(DataConfiguration.DataType.item);
                    break;*/
                case CompletionType.Talk_To_Npc:
                    guiValueTxt.Enabled = true;
                    guiValueSearchBtn.Enabled = true;
                    m_codeSearch[0].setDataType(DataConfiguration.DataType.npc);
                    break;
                case CompletionType.Use_Skill_On_Mob_Type:
                    guiValueTxt.Enabled = true;
                    guiValueSearchBtn.Enabled = true;
                    m_codeSearch[0].setDataType(DataConfiguration.DataType.mob);
                    guiDataTxt.Enabled = true;
                    guiDataSearchBtn.Enabled = true;
                    m_codeSearch[1].setDataType(DataConfiguration.DataType.skill);
                    break;
                case CompletionType.Use_Skill_On_Object:
                    guiValueTxt.Enabled = true;
                    guiValueSearchBtn.Enabled = true;
                    m_codeSearch[0].setDataType(DataConfiguration.DataType.sector_object);
                    guiDataTxt.Enabled = true;
                    guiDataSearchBtn.Enabled = true;
                    m_codeSearch[1].setDataType(DataConfiguration.DataType.skill);
                    break;
            }
        }

        /// <summary>
        /// Validate the entered data and close the dialog
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void onOk(object sender, EventArgs e)
        {
            m_completion = new Completion();
            String value = "";
            String data = "";
            Int32 count = -1;
            CompletionType completionType = (CompletionType)guiTypeCbo.SelectedItem;
            m_completion.setCompletionType(completionType);
            if (guiValueTxt.Enabled)
            {
                value = guiValueTxt.Text;
            }
            if (guiDataTxt.Enabled)
            {
                data = guiDataTxt.Text;
            }
            if (guiAmountTxt.Enabled)
            {
                if (!Int32.TryParse(guiAmountTxt.Text, out count))
                {
                    count = -1;
                }
            }

            switch (completionType)
            {
                case CompletionType.Arrive_At:
                    m_completion.setValue(value);
                    break;
                case CompletionType.Nav_Message:
                    m_completion.setValue(value);
                    m_completion.setCount(count);
                    break;
                case CompletionType.Proximity_To_Space_Npc:
                    m_completion.setValue(value);
                    break;
                case CompletionType.Talk_Space_Npc:
                    m_completion.setValue(value);
                    break;
                case CompletionType.Fight_Mob:
                    m_completion.setValue(value);
                    m_completion.setCount(count);
                    break;
                case CompletionType.Give_Credits:
                    m_completion.setValue(count.ToString());
                    break;
                case CompletionType.Give_Item:
                    m_completion.setValue(value);
                    m_completion.setCount(count);
                    break;
                case CompletionType.Nearest_Nav:
                    m_completion.setValue(value);
                    break;
                case CompletionType.Obtain_Items:
                    m_completion.setValue(value);
                    break;
                /*case CompletionType.Obtain_Items_At_Location:
                    m_completion.setValue(value);
                    m_completion.setData(data);
                    break;*/
                case CompletionType.Possess_Item:
                    m_completion.setValue(value);
                    m_completion.setCount(count);
                    break;
                case CompletionType.Receive_Item:
                    m_completion.setValue(count.ToString());
                    break;
                case CompletionType.Current_Sector:
                    m_completion.setValue(value);
                    break;
                /*case CompletionType.Take_Item_To_Location:
                    m_completion.setValue(value);
                    m_completion.setData(data);
                    break;*/
                case CompletionType.Talk_To_Npc:
                    m_completion.setValue(value);
                    break;
                case CompletionType.Use_Skill_On_Mob_Type:
                    m_completion.setValue(value);
                    m_completion.setData(data);
                    break;
                case CompletionType.Use_Skill_On_Object:
                    m_completion.setValue(value);
                    m_completion.setData(data);
                    break;
            }

            String error;
            m_completion.addValidations();
            if (Database.DataConfiguration.validate(out error))
                //&& MissionEditor.Program.m_frmMission.isMissionValid(out error))
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

        public Boolean getValues(out Completion completion)
        {
            completion = m_completion;
            return m_madeSelection;
        }

    }
}
