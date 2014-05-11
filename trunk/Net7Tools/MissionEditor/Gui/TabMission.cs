using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using MissionEditor.Nodes;

namespace MissionEditor.Gui
{
    public partial class TabMission : UserControl
    {
        private FrmMission m_parent;
        private Mission m_mission;
        private DlgConditions m_dlgConditions;
        private DlgStages m_dlgStages;

        public TabMission(FrmMission parent)
        {
            InitializeComponent();
            m_parent = parent;
            m_mission = null;
            m_dlgConditions = new DlgConditions();
            m_dlgStages = new DlgStages();
            CommonTools.Enumeration.AddSortedByName<CommonTools.MissionType>(guiTypeCbo);

            new CommonTools.Gui.TableButtonHandler(guiConditionsTbl, guiConditionsAddBtn, guiConditionsRemoveBtn, guiConditionsEditBtn, guiConditionUpBtn, guiConditionDownBtn);
            guiConditionUpBtn.Click += new EventHandler(onConditionReordered);
            guiConditionDownBtn.Click += new EventHandler(onConditionReordered);

            new CommonTools.Gui.TableButtonHandler(guiStagesTbl, guiStagesAddBtn, guiStagesRemoveBtn, null, guiStageUpBtn, guiStageDownBtn);
            guiStageUpBtn.Click += new EventHandler(onStageReordered);
            guiStageDownBtn.Click += new EventHandler(onStageReordered);
        }

        public void setMission(Mission mission)
        {
            m_mission = mission;
            guiIdTxt.Text = m_mission.getId();
            guiTypeCbo.SelectedItem = m_mission.getType();
            guiKeyTxt.Text = m_mission.getKey();
            guiNameTxt.Text = m_mission.getName();
            guiSummaryTxt.Text = m_mission.getSummary();
            guiTimeTxt.Text = m_mission.getAllowedTime().ToString();
            guiForfeitableChk.Checked = m_mission.isForfeitable();
            guiConditionsTbl.Items.Clear();
            if (m_mission.hasConditions())
            {
                foreach (Condition condition in m_mission.getConditions())
                {
                    ListViewItem item = new ListViewItem(new String[] { condition.getConditionType().ToString(),
                                                                        condition.getFormattedValue() });
                    guiConditionsTbl.Items.Add(item);
                    item.Tag = condition;
                }
                guiConditionsTbl.Items[0].Selected = true;
            }
            guiStagesTbl.Items.Clear();
            if (m_mission.hasStages())
            {
                foreach (Stage stage in m_mission.getStages())
                {
                    ListViewItem item = new ListViewItem(new String[] { stage.getId(),
                                                                        stage.getDescription() });
                    guiStagesTbl.Items.Add(item);
                    item.Tag = stage;
                }
                guiStagesTbl.Items[0].Selected = true;
            }
        }

        public void setMissionType(CommonTools.MissionType missionType, String missionKey)
        {
            m_mission.setType(missionType);
            m_mission.setKey(missionKey);
            guiTypeCbo.SelectedItem = missionType;
            guiKeyTxt.Text = missionKey;
        }

        public ListView getStageTable()
        {
            return guiStagesTbl;
        }

        private void onSimpleChanged(object sender, EventArgs e)
        {
            m_parent.onChanged();
        }

        private void onChanged(object sender, EventArgs e)
        {
            String widget = ((Control)sender).Name;
            if (widget.Equals(guiKeyTxt.Name)) m_mission.setKey(guiKeyTxt.Text);
            else if (widget.Equals(guiNameTxt.Name)) m_mission.setName(guiNameTxt.Text);
            else if (widget.Equals(guiTypeCbo.Name)) m_mission.setType((CommonTools.MissionType)guiTypeCbo.SelectedItem);
            else if (widget.Equals(guiKeyTxt.Name)) m_mission.setKey(guiKeyTxt.Text);
            else if (widget.Equals(guiSummaryTxt.Name)) m_mission.setSummary(guiSummaryTxt.Text);
            else if (widget.Equals(guiTimeTxt.Name))
            {
                Int32 time;
                if (!Int32.TryParse(guiTimeTxt.Text, out time))
                {
                    time = 0;
                }
                m_mission.setAllowedTime(time);
            }
            else if (widget.Equals(guiForfeitableChk.Name)) m_mission.setForfeitable(guiForfeitableChk.Checked);

            m_parent.onChanged();
        }

        /// <summary>
        /// Ensure that only numbers can be specified within the Time field
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void onTimeKeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar < '0' || e.KeyChar > '9')
            {
                e.Handled = true;
            }
        }

        private void onConditionSelected(object sender, EventArgs e)
        {
            MessageBox.Show("onConditionSelected");
            ListView conditionsTable = (ListView)sender;
            guiConditionsRemoveBtn.Enabled = conditionsTable.SelectedItems.Count != 0;
            //guiConditionsEditBtn.Enabled = conditionsTable.SelectedItems.Count == 1;
        }

        private void onConditionAdd(object sender, EventArgs e)
        {
            m_dlgConditions.ShowDialog();
            Condition condition;
            if (m_dlgConditions.getValues(out condition))
            {
                m_parent.onChanged();
                m_mission.addCondition(condition);
                ListViewItem item = new ListViewItem(new String[] { condition.getConditionType().ToString(),
                                                                    condition.getFormattedValue() });
                item.Tag = condition;
                guiConditionsTbl.Items.Add(item);
            }
        }

        private void onConditionEdit(object sender, EventArgs e)
        {
            m_dlgConditions.editCondition((Condition)guiConditionsTbl.SelectedItems[0].Tag);
            m_dlgConditions.ShowDialog();
            Condition condition;
            if (m_dlgConditions.getValues(out condition))
            {
                m_parent.onChanged();
                guiConditionsTbl.SelectedItems[0].Tag = condition;
            }
        }

        private void onConditionRemove(object sender, EventArgs e)
        {
            m_parent.onChanged();
            foreach (ListViewItem item in guiConditionsTbl.SelectedItems)
            {
                m_mission.removeCondition((Condition)item.Tag);
                guiConditionsTbl.Items.Remove(item);
            }
        }

        private void onConditionReordered(object sender, EventArgs e)
        {
            m_mission.clearConditions();
            int selected = guiConditionsTbl.SelectedItems[0].Index;
            ListViewItem lvItem;
            for (int conditionId = 0; conditionId < guiConditionsTbl.Items.Count; conditionId++)
            {
                lvItem = guiConditionsTbl.Items[conditionId];
                Condition condition = (Condition)lvItem.Tag;
                guiConditionsTbl.Items.RemoveAt(conditionId);
                lvItem = new ListViewItem(new String[] { condition.getConditionType().ToString(),
                                                         condition.getFormattedValue() });
                lvItem.Tag = condition;
                guiConditionsTbl.Items.Insert(conditionId, lvItem);
                if (conditionId == selected)
                {
                    lvItem.Selected = true;
                }
                m_mission.addCondition(condition);
            }
            m_parent.onChanged();
        }

        private void onStageSelected(object sender, EventArgs e)
        {
            /*
            ListView stagesTable = (ListView)sender;
            // TODO: Only enabled when last is selected, until can properly handle stage reordering
            //guiStagesRemoveBtn.Enabled = stagesTable.SelectedItems.Count != 0;
            guiStagesRemoveBtn.Enabled = stagesTable.SelectedIndices.Count == 1 && stagesTable.SelectedIndices[0] == stagesTable.Items.Count - 1;
            // TODO: Edition is disabled until can properly display the value to edit
            //guiStagesEditBtn.Enabled = stagesTable.SelectedItems.Count == 1;
            */

            if (guiStagesTbl.SelectedIndices.Count == 1)
            {
                m_parent.onStageSelected((Stage)guiStagesTbl.SelectedItems[0].Tag);
            }
        }

        public void addStage(Boolean showDialog)
        {
            Int32 stageId = 0;
            if (guiStagesTbl.Items.Count != 0)
            {
                stageId = Int32.Parse(((Stage)guiStagesTbl.Items[guiStagesTbl.Items.Count - 1].Tag).getId()) + 1;
                // TODO: Inserting a stage within the sequence involves updating all the dependants of the renumbered stage IDs
                /*
                if (guiStagesTbl.SelectedItems.Count == 0)
                {
                    stageId = Int32.Parse(((Stage)guiStagesTbl.Items[guiStagesTbl.Items.Count - 1].Tag).getId()) + 1;
                }
                else
                {
                    stageId = Int32.Parse(((Stage)guiStagesTbl.SelectedItems[0].Tag).getId()) + 1;
                }
                */
            }

            Stage stage = null;
            Boolean addNewStage = true;
            if (showDialog)
            {
                m_dlgStages.setId(stageId);
                m_dlgStages.ShowDialog();
                addNewStage = m_dlgStages.getValues(out stage);
            }
            else
            {
                stage = new Stage();
                stage.setId(stageId.ToString());
            }

            if (addNewStage)
            {
                m_parent.onChanged();
                m_mission.addStage(stage);
                ListViewItem item = new ListViewItem(new String[] { stage.getId(),
                                                                    stage.getDescription() });
                item.Tag = stage;
                guiStagesTbl.Items.Add(item);
                m_parent.onChangedStages();
                item.Selected = true;
            }
        }

        private void onStageAdd(object sender, EventArgs e)
        {
            addStage(guiStagesTbl.Items.Count != 0); // Description not needed for stage 0
        }

        private void onStageEdit(object sender, EventArgs e)
        {
            m_dlgStages.editStage((Stage)guiStagesTbl.SelectedItems[0].Tag);
            m_parent.onChanged();
        }

        private void onStageRemove(object sender, EventArgs e)
        {
            foreach (ListViewItem item in guiStagesTbl.SelectedItems)
            {
                int selectedIndex = item.Index;
                m_mission.removeStage((Stage)item.Tag);
                guiStagesTbl.Items.Remove(item);
                if (selectedIndex >= guiStagesTbl.Items.Count)
                {
                    selectedIndex--;
                }
                if (selectedIndex != -1)
                {
                    guiStagesTbl.Items[selectedIndex].Selected = true;
                    onStageReordered(null, null);
                }
                else
                {
                    m_parent.onChanged();
                    m_parent.onChangedStages();
                }
            }
        }

        private void onStageReordered(object sender, EventArgs e)
        {
            m_mission.clearStages();
            int selected = guiStagesTbl.SelectedItems[0].Index;
            ListViewItem lvItem;
            for (int stageId = 0; stageId < guiStagesTbl.Items.Count; stageId++)
            {
                lvItem = guiStagesTbl.Items[stageId];
                Stage stage = (Stage)lvItem.Tag;
                stage.setId(stageId.ToString());
                guiStagesTbl.Items.RemoveAt(stageId);
                lvItem = new ListViewItem(new String[] { stage.getId(),
                                                         stage.getDescription() });
                lvItem.Tag = stage;
                guiStagesTbl.Items.Insert(stageId, lvItem);
                if (stageId == selected)
                {
                    lvItem.Selected = true;
                }
                m_mission.addStage(stage);
            }
            m_parent.onChanged();
            m_parent.onChangedStages();
        }

        private void onKeySearch(object sender, EventArgs e)
        {
            String code = "";
            switch ((CommonTools.MissionType)guiTypeCbo.SelectedItem)
            {
                case CommonTools.MissionType.Npc:
                    code = MissionEditor.Database.DataConfiguration.search(MissionEditor.Database.DataConfiguration.DataType.npc);
                    break;
                case CommonTools.MissionType.Sector:
                    code = MissionEditor.Database.DataConfiguration.search(MissionEditor.Database.DataConfiguration.DataType.sector);
                    break;
            }
            if (code.Length != 0)
            {
                guiKeyTxt.Text = code;
            }
        }

    }
}
