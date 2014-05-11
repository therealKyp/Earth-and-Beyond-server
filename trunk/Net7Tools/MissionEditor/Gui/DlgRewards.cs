using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using MissionEditor.Nodes;
using CommonTools;
using MissionEditor.Database;

namespace MissionEditor.Gui
{
    public partial class DlgRewards : Form
    {
        private Boolean m_madeSelection;
        private Reward m_reward;
        CodeDescSearch m_codeSearch;

        public DlgRewards()
        {
            InitializeComponent();

            m_reward = null;
            m_codeSearch = new CodeDescSearch(guiCodeTxt, guiCodeDescriptionTxt, guiCodeSearchBtn);
            guiTypeCbo.Items.Clear();
            CommonTools.Enumeration.AddSortedByName<CommonTools.RewardType>(guiTypeCbo);
            guiTypeCbo.SelectedIndex = 0;
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);
            m_madeSelection = false;
            guiTypeCbo.Focus();
        }

        public void editReward(Reward reward)
        {
            m_reward = reward;
            // TODO: Display the Reward for edit
        }

        private void onTypeSelected(object sender, EventArgs e)
        {
            guiAmountTxt.Enabled = false;
            guiCodeTxt.Enabled = false;
            guiCodeSearchBtn.Enabled = false;

            RewardType rewardType = (RewardType)guiTypeCbo.SelectedItem;
            switch (rewardType)
            {
                case RewardType.Credits:
                case RewardType.Explore_XP:
                case RewardType.Combat_XP:
                case RewardType.Trade_XP:
                    guiAmountTxt.Enabled = true;
                    break;
                case RewardType.Award_Skill:
                    guiCodeTxt.Enabled = true;
                    guiCodeSearchBtn.Enabled = true;
                    m_codeSearch.setDataType(DataConfiguration.DataType.skill);
                    break;
                case RewardType.Faction:
                    guiCodeTxt.Enabled = true;
                    guiCodeSearchBtn.Enabled = true;
                    guiAmountTxt.Enabled = true;
                    m_codeSearch.setDataType(DataConfiguration.DataType.faction);
                    break;
                case RewardType.Item_ID:
                    guiCodeTxt.Enabled = true;
                    guiCodeSearchBtn.Enabled = true;
                    m_codeSearch.setDataType(DataConfiguration.DataType.item);
                    break;
                case RewardType.Hull_Upgrade:
                    guiAmountTxt.Enabled = true;
                    break;
                case RewardType.Advance_Mission:
                    guiCodeTxt.Enabled = true;
                    guiCodeSearchBtn.Enabled = true;
                    m_codeSearch.setDataType(DataConfiguration.DataType.mission);
                    break;
                case RewardType.Run_Script:
                    guiAmountTxt.Enabled = true;
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
            m_reward = new Reward();
            /*if (m_reward == null)
            {
                m_reward = new Reward();
            }
            else
            {
                m_reward.clear();
            }*/
            RewardType rewardType = (RewardType)guiTypeCbo.SelectedItem;
            m_reward.setRewardType(rewardType);
            switch (rewardType)
            {
                case RewardType.Award_Skill:
                    m_reward.setValue(guiCodeTxt.Text);
                    break;
                case RewardType.Credits:
                case RewardType.Explore_XP:
                case RewardType.Combat_XP:
                case RewardType.Trade_XP:
                    m_reward.setValue(guiAmountTxt.Text);
                    break;
                case RewardType.Faction:
                    m_reward.setValue(guiAmountTxt.Text);
                    m_reward.setFlag(guiCodeTxt.Text);
                    break;
                case RewardType.Item_ID:
                    m_reward.setValue(guiCodeTxt.Text);
                    break;
                case RewardType.Advance_Mission:
                    m_reward.setValue(guiCodeTxt.Text);
                    break;
                case RewardType.Hull_Upgrade:
                case RewardType.Run_Script:
                    m_reward.setValue(guiAmountTxt.Text);
                    break;
            }

            String error;
            m_reward.addValidations();
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

        public Boolean getValues(out Reward reward)
        {
            reward = m_reward;
            return m_madeSelection;
        }
    }
}
