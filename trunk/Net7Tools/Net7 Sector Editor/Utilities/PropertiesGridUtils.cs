using System;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Reflection;
using System.Collections.Specialized;
using System.Drawing.Design;
using System.Drawing;
using System.Resources;
using System.Windows.Forms.Design;

using N7.GUI;

namespace N7.Utilities
{
    internal class HE_GlobalVars
    {
        internal static string[] _ListofTypes = new string[] {"Mobs", "Planets", "Stargates", "Starbases", "Decorations", "Harvestables"};
        internal static string[] _ListofFactions;
        internal static string[] _ListofFieldTypes = new string[] { "Random", "Ring", "Donut", "Cylinder", "Sphere", "Gas Cloud Clump"};
        internal static string[] _ListofNavTypes = new string[] { "0", "1", "2" };
        internal static string[] _ListofLevels = new string[] { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
        internal static string[] _listofSectorTypes = new string[] { "Space Sector", "Rocky Planet Surface", "Gas Giant Surface"};
    }

    public class TypesConverter : StringConverter
    {

        public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
        {
            //true means show a combobox
            return true;
        }

        public override bool GetStandardValuesExclusive(ITypeDescriptorContext context)
        {
            //true will limit to list. false will show the list, but allow free-form entry
            return true;
        }

        public override
            System.ComponentModel.TypeConverter.StandardValuesCollection
            GetStandardValues(ITypeDescriptorContext context)
        {
            return new StandardValuesCollection(HE_GlobalVars._ListofTypes);
        }
    }

    public class FactionConverter : StringConverter
    {

        public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
        {
            //true means show a combobox
            return true;
        }

        public override bool GetStandardValuesExclusive(ITypeDescriptorContext context)
        {
            //true will limit to list. false will show the list, but allow free-form entry
            return true;
        }

        public override
            System.ComponentModel.TypeConverter.StandardValuesCollection
            GetStandardValues(ITypeDescriptorContext context)
        {
            return new StandardValuesCollection(HE_GlobalVars._ListofFactions);
        }
    }

    public class FieldTypeConverter : StringConverter
    {

        public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
        {
            //true means show a combobox
            return true;
        }

        public override bool GetStandardValuesExclusive(ITypeDescriptorContext context)
        {
            //true will limit to list. false will show the list, but allow free-form entry
            return true;
        }

        public override
            System.ComponentModel.TypeConverter.StandardValuesCollection
            GetStandardValues(ITypeDescriptorContext context)
        {
            return new StandardValuesCollection(HE_GlobalVars._ListofFieldTypes);
        }
    }

    public class NavTypeConverter : StringConverter
    {

        public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
        {
            //true means show a combobox
            return true;
        }

        public override bool GetStandardValuesExclusive(ITypeDescriptorContext context)
        {
            //true will limit to list. false will show the list, but allow free-form entry
            return true;
        }

        public override
            System.ComponentModel.TypeConverter.StandardValuesCollection
            GetStandardValues(ITypeDescriptorContext context)
        {
            return new StandardValuesCollection(HE_GlobalVars._ListofNavTypes);
        }
    }

    public class LevelConverter : StringConverter
    {

        public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
        {
            //true means show a combobox
            return true;
        }

        public override bool GetStandardValuesExclusive(ITypeDescriptorContext context)
        {
            //true will limit to list. false will show the list, but allow free-form entry
            return true;
        }

        public override
            System.ComponentModel.TypeConverter.StandardValuesCollection
            GetStandardValues(ITypeDescriptorContext context)
        {
            return new StandardValuesCollection(HE_GlobalVars._ListofLevels);
        }
    }

    public class ContrastEditor : UITypeEditor
    {
        public override UITypeEditorEditStyle GetEditStyle(
            ITypeDescriptorContext context)
        {
            return UITypeEditorEditStyle.DropDown;
        }

        public override object EditValue(ITypeDescriptorContext context,
            IServiceProvider provider, object value)
        {
            IWindowsFormsEditorService wfes = provider.GetService(
                typeof(IWindowsFormsEditorService)) as
                IWindowsFormsEditorService;

            if (wfes != null)
            {
                frmContrast _frmContrast = new frmContrast();

                _frmContrast.trackBar1.Value = (int)value;
                _frmContrast.BarValue = _frmContrast.trackBar1.Value;
                _frmContrast._wfes = wfes;

                wfes.DropDownControl(_frmContrast);
                value = _frmContrast.BarValue;

            }
            return value;
        }
    }

        public class BaseAssetsEditor : UITypeEditor
        {
            public override UITypeEditorEditStyle GetEditStyle(
                ITypeDescriptorContext context)
            {
                return UITypeEditorEditStyle.Modal;
            }

            public override object EditValue(ITypeDescriptorContext context,
                IServiceProvider provider, object value)
            {
                IWindowsFormsEditorService wfes = provider.GetService(
                    typeof(IWindowsFormsEditorService)) as
                    IWindowsFormsEditorService;

                if (wfes != null)
                {
                    BaseAssets _baseAssets = new BaseAssets();

                    _baseAssets.pgID = (int)value;
                    _baseAssets._wfes = wfes;

                    wfes.ShowDialog(_baseAssets);
                    value = _baseAssets.selectedID;
                }
                return value;
            }
        }

    public class ResTypeEditor : UITypeEditor
    {
        public override UITypeEditorEditStyle GetEditStyle(
            ITypeDescriptorContext context)
        {
            return UITypeEditorEditStyle.Modal;
        }

        public override object EditValue(ITypeDescriptorContext context,
            IServiceProvider provider, object value)
        {
            IWindowsFormsEditorService wfes = provider.GetService(
                typeof(IWindowsFormsEditorService)) as
                IWindowsFormsEditorService;

            if (wfes != null)
            {
                HarvestableResTypes hrt = new HarvestableResTypes();
                hrt._wfes = wfes;

                wfes.ShowDialog(hrt);
                value = "<Collection...>";
            }
            return value;
        }
    }

    public class MobGroupEditor : UITypeEditor
    {
        public override UITypeEditorEditStyle GetEditStyle(
            ITypeDescriptorContext context)
        {
            return UITypeEditorEditStyle.Modal;
        }

        public override object EditValue(ITypeDescriptorContext context,
            IServiceProvider provider, object value)
        {
            IWindowsFormsEditorService wfes = provider.GetService(
                typeof(IWindowsFormsEditorService)) as
                IWindowsFormsEditorService;

            if (wfes != null)
            {
                MobGroup mg = new MobGroup();
                mg._wfes = wfes;

                wfes.ShowDialog(mg);
                value = "<Collection...>";
            }
            return value;
        }
    }

    public class SectorTypeConvertor : StringConverter
    {

        public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
        {
            //true means show a combobox
            return true;
        }

        public override bool GetStandardValuesExclusive(ITypeDescriptorContext context)
        {
            //true will limit to list. false will show the list, but allow free-form entry
            return true;
        }

        public override
            System.ComponentModel.TypeConverter.StandardValuesCollection
            GetStandardValues(ITypeDescriptorContext context)
        {
            return new StandardValuesCollection(HE_GlobalVars._listofSectorTypes);
        }
    }

    public class SoundEffectEditor : UITypeEditor
    {
        public override UITypeEditorEditStyle GetEditStyle(
            ITypeDescriptorContext context)
        {
            return UITypeEditorEditStyle.Modal;
        }

        public override object EditValue(ITypeDescriptorContext context,
            IServiceProvider provider, object value)
        {
            IWindowsFormsEditorService wfes = provider.GetService(
                typeof(IWindowsFormsEditorService)) as
                IWindowsFormsEditorService;

            if (wfes != null)
            {
                SoundEffects _soundEffects = new SoundEffects();

                _soundEffects.pgID = (int)value;
                _soundEffects._wfes = wfes;

                wfes.ShowDialog(_soundEffects);
                value = _soundEffects.selectedID;
            }
            return value;
        }
    }

    public class DestinationEditor : UITypeEditor
    {
        public override UITypeEditorEditStyle GetEditStyle(
            ITypeDescriptorContext context)
        {
            return UITypeEditorEditStyle.Modal;
        }

        public override object EditValue(ITypeDescriptorContext context,
            IServiceProvider provider, object value)
        {
            IWindowsFormsEditorService wfes = provider.GetService(
                typeof(IWindowsFormsEditorService)) as
                IWindowsFormsEditorService;

            if (wfes != null)
            {
                Destination _destination = new Destination();

                _destination.pgID = (int)value;
                _destination._wfes = wfes;

                wfes.ShowDialog(_destination);
                value = _destination.selectedID;
            }
            return value;
        }
    }
}
