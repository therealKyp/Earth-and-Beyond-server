using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace N7.GUI
{
    public partial class OptionsGui : UserControl
    {
        private SectorWindow sectorWindow;
        public OptionsGui(SectorWindow sw)
        {
            sectorWindow = sw;
            InitializeComponent();
        }

        private void OptionsGui_Load(object sender, EventArgs e)
        {
        }

        public void setSectorWindow(SectorWindow sw)
        {
            sectorWindow = sw;
        }

        public void loadAll()
        {
            //Mobs
            mobsAIR_CheckedChanged(null,null);
            mobsExpRange_CheckedChanged(null, null);
            mobsLayer_CheckedChanged(null, null);
            mobsNT0_CheckedChanged(null, null);
            mobsNT1_CheckedChanged(null, null);
            mobsNT2_CheckedChanged(null, null);
            mobsRadarRange_CheckedChanged(null, null);
            mobsSignature_CheckedChanged(null, null);
            mobsTextOverlay_CheckedChanged(null, null);

            //Starbases
            starbasesAIR_CheckedChanged(null, null);
            starbasesExpRange_CheckedChanged(null, null);
            starbasesLayer_CheckedChanged(null, null);
            starbasesNT0_CheckedChanged(null, null);
            starbasesNT1_CheckedChanged(null, null);
            starbasesNT2_CheckedChanged(null, null);
            starbasesRadarRange_CheckedChanged(null, null);
            starbasesSignature_CheckedChanged(null, null);
            starbasesTextOverlay_CheckedChanged(null, null);

            //Stargates
            stargatesAIR_CheckedChanged(null, null);
            stargatesExpRange_CheckedChanged(null, null);
            stargatesLayer_CheckedChanged(null, null);
            stargatesNT0_CheckedChanged(null, null);
            stargatesNT1_CheckedChanged(null, null);
            stargatesNT2_CheckedChanged(null, null);
            stargatesRadarRange_CheckedChanged(null, null);
            stargatesSignature_CheckedChanged(null, null);
            stargatesTextOverlay_CheckedChanged(null, null);

            //Deco
            decoAIR_CheckedChanged(null, null);
            decoExpRange_CheckedChanged(null, null);
            decorationsLayer_CheckedChanged(null, null);
            decoNT0_CheckedChanged(null, null);
            decoNT1_CheckedChanged(null, null);
            decoNT2_CheckedChanged(null, null);
            decorationsRadarRange_CheckedChanged(null, null);
            decorationsSignature_CheckedChanged(null, null);
            decorationsTextOverlay_CheckedChanged(null, null);

            //Harvestable
            harvAIR_CheckedChanged(null, null);
            harvExpRange_CheckedChanged(null, null);
            harvestablesLayer_CheckedChanged(null, null);
            harvNT0_CheckedChanged(null, null);
            harvNT1_CheckedChanged(null, null);
            harvNT2_CheckedChanged(null, null);
            harvestablesRadarRange_CheckedChanged(null, null);
            harvestablesSignature_CheckedChanged(null, null);
            harvestablesTextOverlay_CheckedChanged(null, null);
        }

        #region Mobs Options
        private void mobsLayer_CheckedChanged(object sender, EventArgs e)
        {
            if (mobsLayer.Checked == true)
            {
                sectorWindow.showLayer(0);
            }
            else
            {
                sectorWindow.hideLayer(0);
            }
        }

        private void mobsTextOverlay_CheckedChanged(object sender, EventArgs e)
        {
            if (mobsTextOverlay.Checked == true)
            {
                sectorWindow.turnOnText(0);
            }
            else
            {
                sectorWindow.turnOffText(0);
            }
        }

        private void mobsRadarRange_CheckedChanged(object sender, EventArgs e)
        {
            if (mobsRadarRange.Checked == true)
            {
                sectorWindow.radarRangeOn(0);
            }
            else
            {
                sectorWindow.radarRangeOff(0);
            }
        }

        private void mobsSignature_CheckedChanged(object sender, EventArgs e)
        {
            if (mobsSignature.Checked == true)
            {
                sectorWindow.SignatureOn(0);
            }
            else
            {
                sectorWindow.SignatureOff(0);
            }
        }

        private void mobsNT0_CheckedChanged(object sender, EventArgs e)
        {
            if (mobsNT0.Checked == true)
            {
                sectorWindow.navTypeZeroOn(0);
            }
            else
            {
                sectorWindow.navTypeZeroOff(0);
            }
        }

        private void mobsNT1_CheckedChanged(object sender, EventArgs e)
        {
            if (mobsNT1.Checked == true)
            {
                sectorWindow.navTypeOneOn(0);
            }
            else
            {
                sectorWindow.navTypeOneOff(0);
            }
        }

        private void mobsNT2_CheckedChanged(object sender, EventArgs e)
        {
            if (mobsNT2.Checked == true)
            {
                sectorWindow.navTypeTwoOn(0);
            }
            else
            {
                sectorWindow.navTypeTwoOff(0);
            }
        }

        private void mobsAIR_CheckedChanged(object sender, EventArgs e)
        {
            if (mobsAIR.Checked == true)
            {
                sectorWindow.appearsInRadarOn(0);
            }
            else
            {
                sectorWindow.appearsInRadarOff(0);
            }
        }

        private void mobsExpRange_CheckedChanged(object sender, EventArgs e)
        {
            if (mobsExpRange.Checked == true)
            {
                sectorWindow.explorationRangeOn(0);
            }
            else
            {
                sectorWindow.explorationRangeOff(0);
            }
        }
        #endregion

        #region Planets Options
        private void planetsLayer_CheckedChanged(object sender, EventArgs e)
        {
            Console.Out.WriteLine("Testing planets");
            if (planetsLayer.Checked == true)
            {
                sectorWindow.showLayer(3);
                Console.Out.WriteLine("Testing planets 1");
            }
            else
            {
                Console.Out.WriteLine("Testing planets 2");
                sectorWindow.hideLayer(3);
            }
        }

        private void planetsTextOverlay_CheckedChanged(object sender, EventArgs e)
        {
            if (planetsTextOverlay.Checked == true)
            {
                sectorWindow.turnOnText(3);
            }
            else
            {
                sectorWindow.turnOffText(3);
            }
        }

        private void planetsRadarRange_CheckedChanged(object sender, EventArgs e)
        {
            if (planetsRadarRange.Checked == true)
            {
                sectorWindow.radarRangeOn(3);
            }
            else
            {
                sectorWindow.radarRangeOff(3);
            }
        }

        private void planetsSignature_CheckedChanged(object sender, EventArgs e)
        {
            if (planetsSignature.Checked == true)
            {
                sectorWindow.SignatureOn(3);
            }
            else
            {
                sectorWindow.SignatureOff(3);
            }
        }

        private void planetsNT0_CheckedChanged(object sender, EventArgs e)
        {
            if (planetsNT0.Checked == true)
            {
                sectorWindow.navTypeZeroOn(3);
            }
            else
            {
                sectorWindow.navTypeZeroOff(3);
            }
        }

        private void planetsNT1_CheckedChanged(object sender, EventArgs e)
        {
            if (planetsNT1.Checked == true)
            {
                sectorWindow.navTypeOneOn(3);
            }
            else
            {
                sectorWindow.navTypeOneOff(3);
            }
        }

        private void planetsNT2_CheckedChanged(object sender, EventArgs e)
        {
            if (planetsNT2.Checked == true)
            {
                sectorWindow.navTypeTwoOn(3);
            }
            else
            {
                sectorWindow.navTypeTwoOff(3);
            }
        }

        private void planetsAIR_CheckedChanged(object sender, EventArgs e)
        {
            if (planetsAIR.Checked == true)
            {
                sectorWindow.appearsInRadarOn(3);
            }
            else
            {
                sectorWindow.appearsInRadarOff(3);
            }
        }

        private void planetsExpRange_CheckedChanged(object sender, EventArgs e)
        {
            if (planetsExpRange.Checked == true)
            {
                sectorWindow.explorationRangeOn(3);
            }
            else
            {
                sectorWindow.explorationRangeOff(3);
            }
        }
        #endregion

        #region Stargates Options
        private void stargatesLayer_CheckedChanged(object sender, EventArgs e)
        {
            if (stargatesLayer.Checked == true)
            {
                sectorWindow.showLayer(11);
            }
            else
            {
                sectorWindow.hideLayer(11);
            }
        }

        private void stargatesTextOverlay_CheckedChanged(object sender, EventArgs e)
        {
            if (stargatesTextOverlay.Checked == true)
            {
                sectorWindow.turnOnText(11);
            }
            else
            {
                sectorWindow.turnOffText(11);
            }
        }

        private void stargatesRadarRange_CheckedChanged(object sender, EventArgs e)
        {
            if (stargatesRadarRange.Checked == true)
            {
                sectorWindow.radarRangeOn(11);
            }
            else
            {
                sectorWindow.radarRangeOff(11);
            }
        }

        private void stargatesSignature_CheckedChanged(object sender, EventArgs e)
        {
            if (stargatesSignature.Checked == true)
            {
                sectorWindow.SignatureOn(11);
            }
            else
            {
                sectorWindow.SignatureOff(11);
            }
        }

        private void stargatesNT0_CheckedChanged(object sender, EventArgs e)
        {
            if (stargatesNT0.Checked == true)
            {
                sectorWindow.navTypeZeroOn(11);
            }
            else
            {
                sectorWindow.navTypeZeroOff(11);
            }
        }

        private void stargatesNT1_CheckedChanged(object sender, EventArgs e)
        {
            if (stargatesNT1.Checked == true)
            {
                sectorWindow.navTypeOneOn(11);
            }
            else
            {
                sectorWindow.navTypeOneOff(11);
            }
        }

        private void stargatesNT2_CheckedChanged(object sender, EventArgs e)
        {
            if (stargatesNT2.Checked == true)
            {
                sectorWindow.navTypeTwoOn(11);
            }
            else
            {
                sectorWindow.navTypeTwoOff(11);
            }
        }

        private void stargatesAIR_CheckedChanged(object sender, EventArgs e)
        {
            if (stargatesAIR.Checked == true)
            {
                sectorWindow.appearsInRadarOn(11);
            }
            else
            {
                sectorWindow.appearsInRadarOff(11);
            }
        }

        private void stargatesExpRange_CheckedChanged(object sender, EventArgs e)
        {
            if (stargatesExpRange.Checked == true)
            {
                sectorWindow.explorationRangeOn(11);
            }
            else
            {
                sectorWindow.explorationRangeOff(11);
            }
        }
        #endregion

        #region Starbases Options
        private void starbasesLayer_CheckedChanged(object sender, EventArgs e)
        {
            if (starbasesLayer.Checked == true)
            {
                sectorWindow.showLayer(12);
            }
            else
            {
                sectorWindow.hideLayer(12);
            }
        }

        private void starbasesTextOverlay_CheckedChanged(object sender, EventArgs e)
        {
            if (starbasesTextOverlay.Checked == true)
            {
                sectorWindow.turnOnText(12);
            }
            else
            {
                sectorWindow.turnOffText(12);
            }
        }

        private void starbasesRadarRange_CheckedChanged(object sender, EventArgs e)
        {
            if (starbasesRadarRange.Checked == true)
            {
                sectorWindow.radarRangeOn(12);
            }
            else
            {
                sectorWindow.radarRangeOff(12);
            }
        }

        private void starbasesSignature_CheckedChanged(object sender, EventArgs e)
        {
            if (starbasesSignature.Checked == true)
            {
                sectorWindow.SignatureOn(12);
            }
            else
            {
                sectorWindow.SignatureOff(12);
            }
        }
        private void starbasesNT0_CheckedChanged(object sender, EventArgs e)
        {
            if (starbasesNT0.Checked == true)
            {
                sectorWindow.navTypeZeroOn(12);
            }
            else
            {
                sectorWindow.navTypeZeroOff(12);
            }
        }

        private void starbasesNT1_CheckedChanged(object sender, EventArgs e)
        {
            if (starbasesNT1.Checked == true)
            {
                sectorWindow.navTypeOneOn(12);
            }
            else
            {
                sectorWindow.navTypeOneOff(12);
            }
        }

        private void starbasesNT2_CheckedChanged(object sender, EventArgs e)
        {
            if (starbasesNT2.Checked == true)
            {
                sectorWindow.navTypeTwoOn(12);
            }
            else
            {
                sectorWindow.navTypeTwoOff(12);
            }
        }

        private void starbasesAIR_CheckedChanged(object sender, EventArgs e)
        {
            if (starbasesAIR.Checked == true)
            {
                sectorWindow.appearsInRadarOn(12);
            }
            else
            {
                sectorWindow.appearsInRadarOff(12);
            }
        }

        private void starbasesExpRange_CheckedChanged(object sender, EventArgs e)
        {
            if (starbasesExpRange.Checked == true)
            {
                sectorWindow.explorationRangeOn(12);
            }
            else
            {
                sectorWindow.explorationRangeOff(12);
            }
        }
        #endregion

        #region Decorations Options
        private void decorationsLayer_CheckedChanged(object sender, EventArgs e)
        {
            if (decorationsLayer.Checked == true)
            {
                sectorWindow.showLayer(37);
            }
            else
            {
                sectorWindow.hideLayer(37);
            }
        }

        private void decorationsTextOverlay_CheckedChanged(object sender, EventArgs e)
        {
            if (decorationsTextOverlay.Checked == true)
            {
                sectorWindow.turnOnText(37);
            }
            else
            {
                sectorWindow.turnOffText(37);
            }
        }

        private void decorationsRadarRange_CheckedChanged(object sender, EventArgs e)
        {
            if (decorationsRadarRange.Checked == true)
            {
                sectorWindow.radarRangeOn(37);
            }
            else
            {
                sectorWindow.radarRangeOff(37);
            }
        }

        private void decorationsSignature_CheckedChanged(object sender, EventArgs e)
        {
            if (decorationsSignature.Checked == true)
            {
                sectorWindow.SignatureOn(37);
            }
            else
            {
                sectorWindow.SignatureOff(37);
            }
        }

        private void decoNT0_CheckedChanged(object sender, EventArgs e)
        {
            if (decoNT0.Checked == true)
            {
                sectorWindow.navTypeZeroOn(37);
            }
            else
            {
                sectorWindow.navTypeZeroOff(37);
            }
        }

        private void decoNT1_CheckedChanged(object sender, EventArgs e)
        {
            if (decoNT1.Checked == true)
            {
                sectorWindow.navTypeOneOn(37);
            }
            else
            {
                sectorWindow.navTypeOneOff(37);
            }
        }

        private void decoNT2_CheckedChanged(object sender, EventArgs e)
        {
            if (decoNT2.Checked == true)
            {
                sectorWindow.navTypeTwoOn(37);
            }
            else
            {
                sectorWindow.navTypeTwoOff(37);
            }
        }

        private void decoAIR_CheckedChanged(object sender, EventArgs e)
        {
            if (decoAIR.Checked == true)
            {
                sectorWindow.appearsInRadarOn(37);
            }
            else
            {
                sectorWindow.appearsInRadarOff(37);
            }
        }

        private void decoExpRange_CheckedChanged(object sender, EventArgs e)
        {
            if (decoExpRange.Checked == true)
            {
                sectorWindow.explorationRangeOn(37);
            }
            else
            {
                sectorWindow.explorationRangeOff(37);
            }
        }
        #endregion

        #region Harvestables Options
        private void harvestablesLayer_CheckedChanged(object sender, EventArgs e)
        {
            if (harvestablesLayer.Checked == true)
            {
                sectorWindow.showLayer(38);
            }
            else
            {
                sectorWindow.hideLayer(38);
            }
        }

        private void harvestablesTextOverlay_CheckedChanged(object sender, EventArgs e)
        {
            if (harvestablesTextOverlay.Checked == true)
            {
                sectorWindow.turnOnText(38);
            }
            else
            {
                sectorWindow.turnOffText(38);
            }
        }

        private void harvestablesRadarRange_CheckedChanged(object sender, EventArgs e)
        {
            if (harvestablesRadarRange.Checked == true)
            {
                sectorWindow.radarRangeOn(38);
            }
            else
            {
                sectorWindow.radarRangeOff(38);
            }
        }

        private void harvestablesSignature_CheckedChanged(object sender, EventArgs e)
        {
            if (harvestablesSignature.Checked == true)
            {
                sectorWindow.SignatureOn(38);
            }
            else
            {
                sectorWindow.SignatureOff(38);
            }
        }

        private void harvNT0_CheckedChanged(object sender, EventArgs e)
        {
            if (harvNT0.Checked == true)
            {
                sectorWindow.navTypeZeroOn(38);
            }
            else
            {
                sectorWindow.navTypeZeroOff(38);
            }
        }

        private void harvNT1_CheckedChanged(object sender, EventArgs e)
        {
            if (harvNT1.Checked == true)
            {
                sectorWindow.navTypeOneOn(38);
            }
            else
            {
                sectorWindow.navTypeOneOff(38);
            }
        }

        private void harvNT2_CheckedChanged(object sender, EventArgs e)
        {
            if (harvNT2.Checked == true)
            {
                sectorWindow.navTypeTwoOn(38);
            }
            else
            {
                sectorWindow.navTypeTwoOff(38);
            }
        }

        private void harvAIR_CheckedChanged(object sender, EventArgs e)
        {
            if (harvAIR.Checked == true)
            {
                sectorWindow.appearsInRadarOn(38);
            }
            else
            {
                sectorWindow.appearsInRadarOff(38);
            }
        }

        private void harvExpRange_CheckedChanged(object sender, EventArgs e)
        {
            if (harvExpRange.Checked == true)
            {
                sectorWindow.explorationRangeOn(38);
            }
            else
            {
                sectorWindow.explorationRangeOff(38);
            }
        }
        #endregion
    }
}
