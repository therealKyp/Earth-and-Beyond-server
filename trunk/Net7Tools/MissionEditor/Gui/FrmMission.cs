using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.IO;
using MissionEditor;
using MissionEditor.Nodes;
using CommonTools;
using CommonTools.Database;
using CommonTools.Gui;

namespace MissionEditor.Gui
{
    public partial class FrmMission : Form
    {
        public enum State { View, Edit, Add };
        private Mission m_mission;
        private TabMission m_tabMission;
        private TabStages m_tabStages;
        private DlgSearch m_dlgSearch;
        private DlgEditXml m_dlgEditXml;
        private DlgReport m_dlgReport;
        private State m_state;
        private String m_currentMissionId;
        private Boolean m_fieldChangesMuted;

        public FrmMission()
        {
            InitializeComponent();
            this.Text = this.Text + " " + CommonTools.Gui.LoginData.ApplicationVersion;

            Database.DataConfiguration.init();
            m_mission = new Mission();
            m_dlgEditXml = null;
            m_dlgReport = null;
            m_dlgSearch = new CommonTools.Gui.DlgSearch();
            m_dlgSearch.configure(Net7.Tables.missions);
            m_fieldChangesMuted = false; 

            configureTabs();
            setState(State.View);
        }

        /// <summary>
        /// Create the user control tabs and add them to the editor
        /// </summary>
        private void configureTabs()
        {
            m_tabMission = new TabMission(this);
            guiTabPageMission.Controls.Add(m_tabMission);
            m_tabStages = new TabStages(this, m_tabMission.getStageTable());
            guiTabPageStages.Controls.Add(m_tabStages);
        }

        private void setState(State state)
        {
            m_state = state;
            Boolean viewState = m_state.Equals(State.View);
            guiAddBtn.Enabled = viewState;
            guiDeleteBtn.Enabled = viewState;
            guiSaveBtn.Enabled = !viewState;
            guiCancelBtn.Enabled = !viewState;
            guiEditBtn.Enabled = viewState;
            guiSearchBtn.Enabled = viewState;
            guiSearchTxt.Enabled = viewState;
        }

        /// <summary>
        /// Retrieve the mission
        /// </summary>
        public Mission getMission()
        {
            return m_mission;
        }

        /// <summary>
        /// Display the form and load the mission specified via setMissionXml()
        /// </summary>
        /// <param name="e"></param>
        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);
            String firstMissionId = Database.Database.getFirstMissionId();
            if (firstMissionId != null)
            {
                loadMission(firstMissionId);
            }
        }

        public void loadMission(String missionId)
        {
            Boolean wasMuted;
            setMuteFieldChanges(true, out wasMuted);
            Cursor = Cursors.WaitCursor;
            if (missionId != null)
            {
                guiStatusLbl.Text = "Retrieving mission " + missionId;
                this.Refresh();
                m_mission = Database.Database.getMission(missionId);
            }
            if (m_mission == null)
            {
                MessageBox.Show("The mission id '" + missionId + "' does not exist");
            }
            else
            {
                try
                {
                    DateTime start = DateTime.Now;
                    guiStatusLbl.Text = "Parsing mission " + m_mission.getId();
                    guiStatusLbl.Refresh(); // Needed to display the status text
                    m_mission.parseXml();

                    String error;
                    Boolean dataIsValid = Database.DataConfiguration.validate(out error);

                    TimeSpan timeSpan = DateTime.Now - start;
                    guiStatusLbl.Text = timeSpan.TotalMilliseconds.ToString() + " milliseconds";
                    m_tabMission.setMission(m_mission);
                    m_tabStages.setMission(m_mission);

                    if (!dataIsValid)
                    {
                        throw (new Exception(error));
                    }
                    m_currentMissionId = m_mission.getId();
                    guiSearchTxt.Text = m_currentMissionId;
                }
                catch (XmlException xmlEx)
                {
                    MessageBox.Show(xmlEx.Message);
                    //reloadMission(mission.getXml());
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                    //reloadMission(mission.getXml());
                }
            }
            Cursor = Cursors.Default;
            setMuteFieldChanges(wasMuted, out wasMuted);
        }

        private void reloadMission(String xmlMission)
        {
            if (m_dlgEditXml == null)
            {
                m_dlgEditXml = new DlgEditXml();
            }
            m_dlgEditXml.setXml(xmlMission);
            m_dlgEditXml.ShowDialog();
            if (m_dlgEditXml.getValues(out xmlMission))
            {
                setState(State.Edit);
                m_mission.clear();
                m_mission.setXml(xmlMission);
                loadMission(null);
            }
            else
            {
                //this.Close();
            }
        }

        public void setMuteFieldChanges(Boolean mute, out Boolean wasMuted)
        {
            wasMuted = m_fieldChangesMuted;
            m_fieldChangesMuted = mute;
        }

        public Boolean getFieldChangesMuted()
        {
            return m_fieldChangesMuted;
        }

        public void onChanged()
        {
            if (m_fieldChangesMuted == false && m_state.Equals(State.View))
            {
                setState(State.Edit);
            }
        }

        public void onChangedStages()
        {
            Boolean wasMuted;
            setMuteFieldChanges(true, out wasMuted);
            m_tabStages.setMission(m_mission);
            setMuteFieldChanges(wasMuted, out wasMuted);
        }

        public void onAddStage(Boolean showDialog)
        {
           m_tabMission.addStage(showDialog);
        }

        public void onStageSelected(Stage stage)
        {
            Boolean wasMuted;
            setMuteFieldChanges(true, out wasMuted);
            m_tabStages.selectStage(stage);
            setMuteFieldChanges(wasMuted, out wasMuted);
        }

        private void onRecordAdd(object sender, EventArgs e)
        {
            setState(State.Add);
            if (m_mission == null)
            {
                m_mission = new Mission();
            }
            m_mission.clear();
            m_mission.setId(Database.Database.getNextMissionId());
            m_mission.setName("");
            Boolean wasMuted;
            setMuteFieldChanges(true, out wasMuted);
            m_tabMission.setMission(m_mission);
            m_tabStages.setMission(m_mission);
            setMuteFieldChanges(wasMuted, out wasMuted);
            guiTabControl.SelectedIndex = 0;
        }

        private void onRecordDelete(object sender, EventArgs e)
        {
            DialogResult dialogResult = MessageBox.Show("Do you really want to delete this record?",
                                                        "Deletion Confirmation",
                                                        MessageBoxButtons.YesNo);
            if (dialogResult.Equals(DialogResult.Yes))
            {
                Database.Database.deleteMission(m_mission);
                loadMission(Database.Database.getFirstMissionId());
            }
        }

        /// <summary>
        /// Perform a full validation on the currently loaded mission
        /// </summary>
        /// <param name="error">The error message (if any)</param>
        /// <returns>Whether the mission is valid</returns>
        public Boolean isMissionValid(out String error)
        {
            // Not used
            m_mission.addFullValidations();
            return Database.DataConfiguration.validate(out error);
        }

        private void onRecordSave(object sender, EventArgs e)
        {
            m_mission.addFullValidations();
            String error;
            if (Database.DataConfiguration.validate(out error))
            {
                if (m_state.Equals(State.Add))
                {
                    Database.Database.setMission(m_mission, true);
                    m_tabMission.setMission(m_mission);
                    m_currentMissionId = m_mission.getId();
                    guiSearchTxt.Text = m_currentMissionId;
                }
                else
                {
                    Database.Database.setMission(m_mission, false);
                }
                setState(State.View);
            }
            else
            {
                MessageBox.Show(error);
            }
        }

        private void onRecordUndo(object sender, EventArgs e)
        {
            setState(State.View);
            loadMission(m_currentMissionId);
        }

        private void onRecordEdit(object sender, EventArgs e)
        {
            reloadMission(m_mission.getXML());
            /*
            if (m_dlgEditXml == null)
            {
                m_dlgEditXml = new DlgEditXml();
            }
            m_dlgEditXml.setXml(m_mission.getXML());
            m_dlgEditXml.ShowDialog();
            String xml;
            if (m_dlgEditXml.getValues(out xml))
            {
                reloadMission(xml);
            }*/
        }

        private void onRecordSearch(object sender, EventArgs e)
        {
            m_dlgSearch.ShowDialog();
            String selectedId = m_dlgSearch.getSelectedId();
            if (selectedId.Length != 0)
            {
                guiTabControl.SelectedIndex = 0;
                guiSearchTxt.Text = selectedId;
                loadMission(selectedId);
            }
        }

        private void onRecordSearch(object sender, KeyEventArgs e)
        {
            if (e.KeyCode.Equals(Keys.Enter))
            {
                guiTabControl.SelectedIndex = 0;
                loadMission(guiSearchTxt.Text);
            }
        }

        private void onRecordReport(object sender, EventArgs e)
        {
            Cursor = Cursors.WaitCursor;
            if (m_dlgReport == null)
            {
                m_dlgReport = new DlgReport();
            }

            m_dlgReport.set(m_mission.getReport());
            Cursor = Cursors.Default;
            m_dlgReport.ShowDialog();
        }

        public void setMissionType(MissionType missionType, String missionKey)
        {
            m_tabMission.setMissionType(missionType, missionKey);
            onChanged();
        }

        private void onExit(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}