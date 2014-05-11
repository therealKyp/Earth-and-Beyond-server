using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;

using Gajatko.IniFiles;

namespace EnBIniParser
{
    class EffectsParser
    {
        private String effectClass;
        private String description;
        private int startLinkID = -1;
        private int nextLinkID = -1;
        private int baseAssetID = -1;
        private String soundFxFile;

        public EffectsParser(int start, int end)
        {
            for (int i = start; i < end; i++)
            {
                try
                {
                    IniFileReader reader = new IniFileReader("C:\\Documents and Settings\\Josh\\Desktop\\effect.ini");
                    IniFileSectionStart test = reader.GotoSection("EFFECT-"+i);
                    List<IniFileValue> test2 = reader.ReadSectionValues();

                    //Console.Out.WriteLine(test.SectionName);
                    foreach (IniFileValue r in test2)
                    {
                        sectionLookup(test, r);
                    }

                    int effectID = int.Parse(test.SectionName.Replace("EFFECT-", ""));

                    String query = "INSERT INTO effects SET effect_id='"+effectID+"', effect_class='"+effectClass+"', descriiption='"+description+"', "+
                        "start_link_id='"+startLinkID+"', next_link_id='"+nextLinkID+"', base_asset_id='"+baseAssetID+"', "+
                        "sound_fx_file='"+soundFxFile+"';";

                    DataTable effectInsert = Database.executeQuery(Database.DatabaseName.net7, query);
                    Console.Out.WriteLine(query);
                    reader.Close();
                }
                catch (Exception e)
                {
                    //Console.Out.WriteLine(e+", "+ e.Message);
                    //throw;
                }
            }

            Console.Out.WriteLine("***** Finished *****");
        }

        private void sectionLookup(IniFileSectionStart test, IniFileValue r)
        {
            switch (r.Key.ToString())
            {
                case "EffectClass":
                    //valueLookup(r);
                    Console.Out.WriteLine(r.Value);
                    effectClass = r.Value.ToString();
                    break;
                case "Description":
                    description = r.Value.ToString();
                    break;
                case "StartLinkID":
                    startLinkID = int.Parse(r.Value.ToString());
                    break;
                case "NextLinkID":
                    nextLinkID = int.Parse(r.Value.ToString());
                    break;
                case "BaseAssetID":
                    baseAssetID = int.Parse(r.Value.ToString());
                    break;
                case "SFX":
                    soundFxFile = r.Value.ToString();
                    break;
            }   
        }

        private void valueLookup(IniFileValue r)
        {
            switch (r.Value.ToString())
            {
                case "RObjEffect":
                    break;
                case "PassEffect":
                    break;
                case "LineEffect":
                    break;
                case "RObjLookAtEffect":
                    break;
                case "StraightSeekEffect":
                    break;
                case "ParticleJetEffect":
                    break;
                case "SplineRandomSeekEffect":
                    break;
                case "AttachEffect":
                    break;
                case "SoundEffect":
                    break;
                case "SpiralSeekEffect":
                    break;
                case "WarpEffect":
                    break;
                case "LinearRandomSeekEffect":
                    break;
                case "RenderStateEffect":
                    break;
                default:
                    Console.Out.WriteLine(r.Value);
                    break;
            }
        }
    }
}
