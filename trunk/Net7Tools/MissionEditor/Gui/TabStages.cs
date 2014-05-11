using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using MissionEditor.Nodes;
using System.IO;
using CommonTools;
using System.Xml;
using CommonTools.Database;

namespace MissionEditor.Gui
{
    public partial class TabStages : UserControl
    {
        private FrmMission m_parent;
        private Mission m_mission;
        private Stage m_stage;
        private DlgRewards m_dlgRewards;
        private DlgCompletions m_dlgCompletions;
        private TalkTreeEditor.FrmTalkTree m_frmTalkTree;
        private ListView m_stageTable;

        public TabStages(FrmMission parent, ListView stageTable)
        {
            InitializeComponent();
            m_parent = parent;
            m_stageTable = stageTable;
            m_mission = null;
            m_stage = null;
            m_dlgRewards = new DlgRewards();
            m_dlgCompletions = new DlgCompletions();
            m_frmTalkTree = new TalkTreeEditor.FrmTalkTree();

            new CommonTools.Gui.TableButtonHandler(guiCompletionsTbl, guiCompletionsAddBtn, guiCompletionsRemoveBtn, guiCompletionsEditBtn, guiCompletionUpBtn, guiCompletionDownBtn);
            guiCompletionUpBtn.Click += new EventHandler(onCompletionReordered);
            guiCompletionDownBtn.Click += new EventHandler(onCompletionReordered);

            new CommonTools.Gui.TableButtonHandler(guiRewardsTbl, guiRewardsAddBtn, guiRewardsRemoveBtn, guiRewardsEditBtn, guiRewardUpBtn, guiRewardDownBtn);
            guiRewardUpBtn.Click += new EventHandler(onRewardReordered);
            guiRewardDownBtn.Click += new EventHandler(onRewardReordered);
        }

        public void setMission(Mission mission)
        {
            m_mission = mission;
            guiStagesCbo.Items.Clear();
            guiDescriptionTxt.Text = "";
            guiCompletionsTbl.Items.Clear();
            guiRewardsTbl.Items.Clear();
            if (m_mission.hasStages())
            {
                foreach (Stage stage in m_mission.getStages())
                {
                    guiStagesCbo.Items.Add(stage);
                }
                selectStage(m_mission.getStages()[0]);
            }
        }

        public void selectStage(Stage stage)
        {
            Boolean wasMuted;
            m_parent.setMuteFieldChanges(true, out wasMuted);
            m_stage = stage;
            guiStagesCbo.SelectedItem = m_stage;
            guiDescriptionTxt.Enabled = (stage.getId().CompareTo("0") != 0); // Enabled when stage is not zero
            guiDescriptionTxt.Text = m_stage.getDescription();
            guiCompletionsTbl.Items.Clear();
            if (m_stage.hasCompletions())
            {
                foreach (Completion completion in m_stage.getCompletions())
                {
                    ListViewItem item = new ListViewItem(new String[] { completion.getCompletionType().ToString(),
                                                                        completion.getFormattedValue().ToString() });
                    guiCompletionsTbl.Items.Add(item);
                    item.Tag = completion;
                }
                guiCompletionsTbl.Items[0].Selected = true;
            }
            guiRewardsTbl.Items.Clear();
            if (m_stage.hasRewards())
            {
                foreach (Reward reward in m_stage.getRewards())
                {
                    // TODO: Better display of the reward (XP shows 0)
                    ListViewItem item = new ListViewItem(new String[] { reward.getRewardType().ToString(),
                                                                        reward.getFormattedValue() });
                    guiRewardsTbl.Items.Add(item);
                    item.Tag = reward;
                }
                guiRewardsTbl.Items[0].Selected = true;
            }
            m_parent.setMuteFieldChanges(wasMuted, out wasMuted);
        }

        private void onAddStage(object sender, EventArgs e)
        {
            m_parent.onAddStage(false);
        }

        private void onStageSelected(object sender, EventArgs e)
        {
            selectStage((Stage)guiStagesCbo.SelectedItem);
        }

        private void onLeaveDescription(object sender, EventArgs e)
        {
            if (!m_parent.getFieldChangesMuted())
            {
                // Hack to refresh the value of the combobox
                int index = guiStagesCbo.Items.IndexOf(m_stage);
                if (index != -1)
                {
                    guiStagesCbo.Items.RemoveAt(index);
                    guiStagesCbo.Items.Insert(index, m_stage);
                    guiStagesCbo.SelectedIndex = index;
                }
            }
        }

        private void onSimpleChanged(object sender, EventArgs e)
        {
            m_parent.onChanged();
        }

        private void onChanged(object sender, EventArgs e)
        {
            if (!m_parent.getFieldChangesMuted())
            {
                String widgetName = ((Control)sender).Name;
                if (widgetName.Equals(guiDescriptionTxt.Name))
                {
                    m_stage.setDescription(guiDescriptionTxt.Text);

                    // Hack to refresh the value of the stage table
                    foreach (ListViewItem item in m_stageTable.Items)
                    {
                        if (item.Tag.Equals(m_stage))
                        {
                            item.SubItems[1].Text = m_stage.getDescription();
                            break;
                        }
                    }
                }

                m_parent.onChanged();
            }
        }

        private void onCompletionSelected(object sender, EventArgs e)
        {
            MessageBox.Show("onCompletionSelected");
            ListView completionsTable = (ListView)sender;
            guiCompletionsRemoveBtn.Enabled = completionsTable.SelectedItems.Count != 0;
            //guiCompletionsEditBtn.Enabled = completionsTable.SelectedItems.Count == 1;
        }

        private void onCompletionAdd(object sender, EventArgs e)
        {
            m_dlgCompletions.ShowDialog();
            Completion completion;
            if (m_dlgCompletions.getValues(out completion))
            {
                m_parent.onChanged();
                m_stage.addCompletion(completion);
                ListViewItem item = new ListViewItem(new String[] { completion.getCompletionType().ToString(),
                                                                    completion.getFormattedValue().ToString() });
                item.Tag = completion;
                guiCompletionsTbl.Items.Add(item);

                if (m_stage.getId().Equals("0"))
                {
                    if (completion.getCompletionType().Equals(CompletionType.Talk_To_Npc))
                    {
                        m_parent.setMissionType(MissionType.Npc, completion.getValue());
                    }
                }
            }
        }

        private void onCompletionEdit(object sender, EventArgs e)
        {
            m_dlgCompletions.editCompletion((Completion)guiCompletionsTbl.SelectedItems[0].Tag);
            m_parent.onChanged();
        }

        private void onCompletionRemove(object sender, EventArgs e)
        {
            m_parent.onChanged();
            foreach (ListViewItem item in guiCompletionsTbl.SelectedItems)
            {
                m_stage.removeCompletion((Completion)item.Tag);
                guiCompletionsTbl.Items.Remove(item);
            }
        }

        private void onCompletionReordered(object sender, EventArgs e)
        {
            m_stage.clearCompletions();
            int selected = guiCompletionsTbl.SelectedItems[0].Index;
            ListViewItem lvItem;
            for (int completionId = 0; completionId < guiCompletionsTbl.Items.Count; completionId++)
            {
                lvItem = guiCompletionsTbl.Items[completionId];
                Completion completion = (Completion)lvItem.Tag;
                guiCompletionsTbl.Items.RemoveAt(completionId);
                lvItem = new ListViewItem(new String[] { completion.getCompletionType().ToString(),
                                                         completion.getFormattedValue().ToString() });
                lvItem.Tag = completion;
                guiCompletionsTbl.Items.Insert(completionId, lvItem);
                if (completionId == selected)
                {
                    lvItem.Selected = true;
                }
                m_stage.addCompletion(completion);
            }
            m_parent.onChanged();
        }

        private void onRewardSelected(object sender, EventArgs e)
        {
            MessageBox.Show("onRewardSelected");
            ListView rewardsTable = (ListView)sender;
            guiRewardsRemoveBtn.Enabled = rewardsTable.SelectedItems.Count != 0;
            //guiRewardsEditBtn.Enabled = rewardsTable.SelectedItems.Count == 1;
            guiRewardDownBtn.Enabled = rewardsTable.SelectedItems.Count == 1;
            guiRewardUpBtn.Enabled = rewardsTable.SelectedItems.Count == 1;
        }

        private void onRewardsAdd(object sender, EventArgs e)
        {
            m_dlgRewards.ShowDialog();
            Reward reward;
            if (m_dlgRewards.getValues(out reward))
            {
                m_parent.onChanged();
                m_stage.addReward(reward);
                ListViewItem item = new ListViewItem(new String[] { reward.getRewardType().ToString(),
                                                                    reward.getFormattedValue().ToString() });
                item.Tag = reward;
                guiRewardsTbl.Items.Add(item);
            }
        }

        private void onRewardEdit(object sender, EventArgs e)
        {
            m_dlgRewards.editReward((Reward)guiRewardsTbl.SelectedItems[0].Tag);
            m_parent.onChanged();
        }

        private void onRewardsRemove(object sender, EventArgs e)
        {
            m_parent.onChanged();
            foreach (ListViewItem item in guiRewardsTbl.SelectedItems)
            {
                m_stage.removeReward((Reward)item.Tag);
                guiRewardsTbl.Items.Remove(item);
            }
        }

        private void onRewardReordered(object sender, EventArgs e)
        {
            m_stage.clearRewards();
            int selected = guiRewardsTbl.SelectedItems[0].Index;
            ListViewItem lvItem;
            for (int rewardId = 0; rewardId < guiRewardsTbl.Items.Count; rewardId++)
            {
                lvItem = guiRewardsTbl.Items[rewardId];
                Reward reward = (Reward)lvItem.Tag;
                guiRewardsTbl.Items.RemoveAt(rewardId);
                lvItem = new ListViewItem(new String[] { reward.getRewardType().ToString(),
                                                         reward.getFormattedValue().ToString() });
                lvItem.Tag = reward;
                guiRewardsTbl.Items.Insert(rewardId, lvItem);
                if (rewardId == selected)
                {
                    lvItem.Selected = true;
                }
                m_stage.addReward(reward);
            }
            m_parent.onChanged();
        }

        private void onTalkTree(object sender, EventArgs e)
        {
            String conversation = null;
            if (m_stage.hasTalkTrees())
            {
                // Edit the talk tree
                StringWriter stringWriter = new StringWriter();
                m_stage.getTalkTreesXML(stringWriter);
                conversation = stringWriter.ToString();
            }
            m_frmTalkTree.setConversation(conversation);

            // Specify the valid stages (for the goto_stage flag)
            List<CodeValue> stages = new List<CodeValue>();
            Int32 stageId;
            foreach(Stage stage in m_mission.getStages())
            {
                stageId = Int32.Parse(stage.getId());
                stages.Add(CodeValue.FormattedLeft(stageId, stage.getDescription()));
            }
            m_frmTalkTree.setStages(stages);

            m_frmTalkTree.ShowDialog();
            if (m_frmTalkTree.getConversation(out conversation))
            {
                m_parent.onChanged();
                XmlDocument xmlDocument = new XmlDocument();
                xmlDocument.Load(new StringReader("<Chat>" + conversation + "</Chat>"));
                m_stage.clearTalkTrees();
                TalkTree talkTree;
                foreach (XmlNode nodeTree in xmlDocument.DocumentElement.ChildNodes)
                {
                    talkTree = new TalkTree();
                    talkTree.fromXML(nodeTree);
                    m_stage.addTalkTree(talkTree);
                }
            }
        }


    }
}
