using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using MissionEditor.Nodes;
using MissionEditor.Database;
using CommonTools;

namespace MissionEditor.Gui
{
    public partial class DlgConditions : Form
    {
        private Boolean m_madeSelection;
        private Condition m_condition;
        CodeDescSearch m_codeSearch;

        public DlgConditions()
        {
            InitializeComponent();

            m_condition = null;
            m_codeSearch = new CodeDescSearch(guiCodeTxt, guiCodeDescriptionTxt, guiCodeSearchBtn);
            guiTypeCbo.Items.Clear();
            CommonTools.Enumeration.AddSortedByName<CommonTools.ConditionType>(guiTypeCbo);
            guiTypeCbo.SelectedIndex = 0;
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);
            m_madeSelection = false;
            guiTypeCbo.Focus();
            guiTypeCbo.SelectedIndex = 0;
        }

        public void editCondition(Condition condition)
        {
            m_condition = condition;
            guiTypeCbo.SelectedItem = m_condition.getConditionType();
            switch (m_condition.getConditionType())
            {
                case CommonTools.ConditionType.Overall_Level:
                case CommonTools.ConditionType.Combat_Level:
                case CommonTools.ConditionType.Explore_Level:
                case CommonTools.ConditionType.Trade_Level:
                case CommonTools.ConditionType.Hull_Level:
                    guiAmountTxt.Text = m_condition.getValue();
                    break;
                case CommonTools.ConditionType.Faction_Required:
                    guiCodeTxt.Text = m_condition.getValue();
                    guiAmountTxt.Text = m_condition.getFlag();
                    m_codeSearch.setDataType(DataConfiguration.DataType.faction);
                    break;
                case CommonTools.ConditionType.Item_Required:
                    guiCodeTxt.Text = m_condition.getValue();
                    guiAmountTxt.Text = m_condition.getFlag();
                    m_codeSearch.setDataType(DataConfiguration.DataType.item);
                    break;
                case CommonTools.ConditionType.Profession:
                    Professions profession = (Professions)Int32.Parse(m_condition.getValue());
                    guiValueCbo.SelectedItem = profession;
                    break;
                case CommonTools.ConditionType.Race:
                    Races race = (Races)Int32.Parse(m_condition.getValue());
                    guiValueCbo.SelectedItem = race;
                    break;
            }
        }

        private void onTypeSelected(object sender, EventArgs e)
        {
            guiValueCbo.Items.Clear();
            guiValueCbo.Enabled = false;
            guiCodeTxt.Enabled = false;
            guiCodeSearchBtn.Enabled = false;
            guiAmountTxt.Enabled = false;

            CommonTools.ConditionType conditionType = (CommonTools.ConditionType)guiTypeCbo.SelectedItem;
            switch (conditionType)
            {
                case CommonTools.ConditionType.Overall_Level:
                case CommonTools.ConditionType.Combat_Level:
                case CommonTools.ConditionType.Explore_Level:
                case CommonTools.ConditionType.Trade_Level:
                case CommonTools.ConditionType.Hull_Level:
                    guiAmountTxt.Enabled = true;
                    break;
                case CommonTools.ConditionType.Faction_Required:
                    guiCodeTxt.Enabled = true;
                    guiCodeSearchBtn.Enabled = true;
                    guiAmountTxt.Enabled = true;
                    m_codeSearch.setDataType(DataConfiguration.DataType.faction);
                    break;
                case CommonTools.ConditionType.Item_Required:
                    guiCodeTxt.Enabled = true;
                    guiCodeSearchBtn.Enabled = true;
                    guiAmountTxt.Enabled = true;
                    m_codeSearch.setDataType(DataConfiguration.DataType.item);
                    break;
                case CommonTools.ConditionType.Profession:
                    guiValueCbo.Enabled = true;
                    guiValueCbo.Items.Clear();
                    CommonTools.Enumeration.AddSortedByName<Professions>(guiValueCbo);
                    guiValueCbo.SelectedIndex = 0;
                    break;
                case CommonTools.ConditionType.Race:
                    guiValueCbo.Enabled = true;
                    guiValueCbo.Items.Clear();
                    CommonTools.Enumeration.AddSortedByName<Races>(guiValueCbo);
                    guiValueCbo.SelectedIndex = 0;
                    break;
                case CommonTools.ConditionType.Mission_Required:
                    guiCodeTxt.Enabled = true;
                    guiCodeSearchBtn.Enabled = true;
                    m_codeSearch.setDataType(DataConfiguration.DataType.mission);
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
            m_condition = new Condition();
            String value;
            Professions profession;
            Races race;
            CommonTools.ConditionType conditionType = (CommonTools.ConditionType)guiTypeCbo.SelectedItem;
            m_condition.setConditionType(conditionType);
            switch (conditionType)
            {
                case CommonTools.ConditionType.Overall_Level:
                case CommonTools.ConditionType.Combat_Level:
                case CommonTools.ConditionType.Explore_Level:
                case CommonTools.ConditionType.Trade_Level:
                case CommonTools.ConditionType.Hull_Level:
                    m_condition.setValue(guiAmountTxt.Text);
                    break;
                case CommonTools.ConditionType.Faction_Required:
                case CommonTools.ConditionType.Item_Required:
                    m_condition.setValue(guiAmountTxt.Text);
                    m_condition.setFlag(guiCodeTxt.Text);
                    break;
                case CommonTools.ConditionType.Profession:
                    // Impossible to enter an invalid value
                    profession = (Professions)guiValueCbo.SelectedItem;
                    value = ((int)profession).ToString();
                    m_condition.setValue(value);
                    break;
                case CommonTools.ConditionType.Race:
                    // Impossible to enter an invalid value
                    race = (Races) guiValueCbo.SelectedItem;
                    value = ((int)race).ToString();
                    m_condition.setValue(value);
                    break;
                case CommonTools.ConditionType.Mission_Required:
                    m_condition.setValue(guiCodeTxt.Text);
                    break;
            }

            String error;
            m_condition.addValidations();
            // Cannot perform a full validation since we could validate that there's at least
            // one condition when we're in the process of creating that first condition
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

        public Boolean getValues(out Condition condition)
        {
            condition = m_condition;
            return m_madeSelection;
        }

    }
}
