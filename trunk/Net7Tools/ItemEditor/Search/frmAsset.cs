using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Net7_Tools.Database;
using System.IO;


namespace Net7_Tools.Search
{
    public partial class FrmAsset : Form, ISearchDialog
    {
        private String m_selectedItemId;
        private String m_everySubCategories = "-= Every Sub-Categories =-";
        private static String m_sourceDirectory = "";
        private List<String> m_limitedCategory = new List<String>();
        private static String m_fileExtention = "bmp";

        public FrmAsset()
        {
            InitializeComponent();
            assetCategoryCbo.SelectedValueChanged += new EventHandler(assetCategoryCbo_Selection);
            assetSubCategoryCbo.SelectedValueChanged += new EventHandler(assetSubCategoryCbo_Selection); 
            m_selectedItemId = "";
        }

        private void loadData()
        {
            Cursor = Cursors.WaitCursor;
            String[] categories = Database.Database.getAssetCategories();
            assetCategoryCbo.Items.Clear();
            if (m_limitedCategory.Count == 0)
            {
                assetCategoryCbo.Items.AddRange(categories);
            }
            else
            {
                foreach (String category in categories)
                {
                    if (m_limitedCategory.Contains(category))
                    {
                        assetCategoryCbo.Items.Add(category);
                    }
                }
            }
            if (assetCategoryCbo.Items.Count != 0)
            {
                assetCategoryCbo.SelectedItem = assetCategoryCbo.Items[0];
            }
            Cursor = Cursors.Default;
        }

        public void setCategoryLimits(List<String> limitedCategory)
        {
            m_limitedCategory = limitedCategory;
        }

        protected override void OnShown(EventArgs e)
        {
            // Reset the currently selected item when the form is shown (or reshown)
            base.OnShown(e);
            loadData();
            selectItemId(m_selectedItemId);
            assetItems.Focus();
            m_selectedItemId = "";
        }
        /// <summary>
        ///   <para>Select the specified item ID if it exists within the tree.</para>
        /// </summary>
        /// <param name="selectedItemId">The item ID to select.</param>
        public void selectItemId(String selectedItemId)
        {
            String category;
            String subCategory;
            if (Database.Database.getAssetCatSubCatFromId(selectedItemId, out category, out subCategory))
            {
                assetCategoryCbo.SelectedItem = category;
                assetSubCategoryCbo.SelectedItem = subCategory;
                assetItems.Focus();
                int selectedItemIndex = assetItems.Items.IndexOfKey(selectedItemId);
                if (selectedItemIndex != -1)
                {
                    assetItems.Items[selectedItemIndex].Selected = true;
                }
            }
        }

        /// <summary>
        ///   <para>Set the selected item ID.</para>
        /// </summary>
        /// <remarks>This will be used when the form is shown, to
        /// select the item (if it exists) within the tree.</remarks>
        public void setSelectedItemId(String selectedItemId)
        {
            m_selectedItemId = selectedItemId;
        }

        /// <summary>
        ///   <para>Retrieve the selected item ID.</para>
        /// </summary>
        /// <returns>The selected item ID</returns>
        public String getSelectedItemId()
        {
            //String id = DB.getAssetFromDescription(m_selectedItemId);
            return m_selectedItemId;
        }

        public Form getForm()
        {
            return this;
        }

        private void assetCategoryCbo_Selection(object sender, EventArgs e)
        {
            Cursor = Cursors.WaitCursor;
            assetSubCategoryCbo.Items.Clear();
            assetSubCategoryCbo.Items.AddRange(Database.Database.getAssetSubCategories(assetCategoryCbo.SelectedItem.ToString()));
            assetSubCategoryCbo.Items.Add(m_everySubCategories);
            if (assetSubCategoryCbo.Items.Count != 0)
            {
                assetSubCategoryCbo.SelectedItem = assetSubCategoryCbo.Items[0];
            }
            Cursor = Cursors.Default;
        }

        public static Image getImage(String assetFilename, String assetId, String assetDesc, out String cleanedAssetFilename)
        {
            cleanedAssetFilename = assetFilename;
            if (!assetFilename.Equals("NULL") && assetFilename.Contains("."))
            {
                int extention = assetFilename.LastIndexOf('.');
                String baseFilename = assetFilename.Substring(0, extention);
                baseFilename = baseFilename.ToLower();
                cleanedAssetFilename = baseFilename.Substring(0, extention) + "." + m_fileExtention;
                if (m_sourceDirectory.Length == 0)
                {
                    // When running within Visual Studion then ItemEditor\Assets
                    // When running the application directly then .\Assets
                    DirectoryInfo directoryInfo = new DirectoryInfo("Assets");
                    m_sourceDirectory = directoryInfo.Exists ? "Assets\\" : "..\\..\\Assets\\";
                }
                String pathedImage = m_sourceDirectory + cleanedAssetFilename;
                Image image = null;
                try
                {
                    image = Image.FromFile(pathedImage);
                }
                catch (System.IO.FileNotFoundException)
                {
                    try
                    {
                        pathedImage = "2d assets\\" + cleanedAssetFilename;
                        image = Image.FromFile(pathedImage);
                    }
                    catch (System.IO.FileNotFoundException)
                    {

                        System.Console.WriteLine("File '"
                                               + pathedImage
                                               + "' is not found for the asset '"
                                               + assetId
                                               + "': " + assetDesc);
                    }
                }
                return image;
            }
            return null;
        }

        private void assetSubCategoryCbo_Selection(object sender, EventArgs e)
        {
            Cursor = Cursors.WaitCursor;
            String category = assetCategoryCbo.SelectedItem.ToString();
            String subCategory = assetSubCategoryCbo.SelectedItem.ToString();
            if(subCategory.Equals(m_everySubCategories))
            {
                subCategory = null;
            }
            AssetInformation[] assetInformation = Database.Database.getAssetItems(category, subCategory);

            assetItems.Clear();
            assetImages.Images.Clear();
            //assetItems.BackColor = Color.FromArgb(169, 169, 169);

            String filename;
            ImageList imageList = new ImageList();
            Image assetImage;
            for (int index = 0; index < assetInformation.Length; index++)
            {
                filename = assetInformation[index].filename;
                assetImage = getImage(filename,
                                      assetInformation[index].base_id.ToString(),
                                      assetInformation[index].descr,
                                      out filename);

                if (assetImage != null)
                {
                    assetImages.Images.Add(filename, assetImage);
                }
                ListViewItem listViewItem = assetItems.Items.Add(assetInformation[index].base_id.ToString(),
                                     assetInformation[index].descr + " (" + assetInformation[index].base_id.ToString() + ")",
                                     filename);
                listViewItem.ToolTipText = filename;
            }
            assetItems.LargeImageList = assetImages;
            Cursor = Cursors.Default;
        }


        public static void convertImages()
        {
            // This bit resizes the original images and saves them in lowercase filenames
            // Assets from:  http://enb.jfisherstudio.com/Images/basset%20images.rar
            DirectoryInfo di = new DirectoryInfo("..\\..\\Data\\Assets_original\\");
            FileInfo[] rgFiles = di.GetFiles("*.jpg");
            String sourceFile;
            String destFile;
            int extention;
            foreach (FileInfo fileInfo in rgFiles)
            {
                sourceFile = fileInfo.Name;
                extention = sourceFile.LastIndexOf('.');
                sourceFile = sourceFile.Substring(0, extention + 1);
                sourceFile = sourceFile.ToLower();
                destFile = sourceFile + m_fileExtention;
                sourceFile += "jpg";
                resizeImage("..\\..\\Data\\Assets_original\\", sourceFile, "..\\..\\Assets\\", destFile, 128, 128);
            }

        }

        /// <remarks>Source from:  http://blog.paranoidferret.com/?p=11</remarks>
        public static void resizeImage(String sourcePath, String sourceFile, String destPath, String destFile, int width, int height)
        {
            Image imgToResize = Image.FromFile(sourcePath + sourceFile);

            int sourceWidth = imgToResize.Width;
            int sourceHeight = imgToResize.Height;

            float nPercent = 0;
            float nPercentW = 0;
            float nPercentH = 0;

            nPercentW = ((float)width / (float)sourceWidth);
            nPercentH = ((float)height / (float)sourceHeight);

            if (nPercentH < nPercentW)
                nPercent = nPercentH;
            else
                nPercent = nPercentW;

            int destWidth = (int)(sourceWidth * nPercent);
            int destHeight = (int)(sourceHeight * nPercent);

            Bitmap b = new Bitmap(destWidth, destHeight);
            b.SetResolution(imgToResize.HorizontalResolution, imgToResize.VerticalResolution);
            Graphics g = Graphics.FromImage((Image)b);
            g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
            g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;

            g.DrawImage(imgToResize, 0, 0, destWidth, destHeight);
            
            g.Dispose();
            b.Save(destPath + destFile);
        }

        private void onOk(object sender, EventArgs e)
        {
            m_selectedItemId = assetItems.SelectedItems.Count == 0 ? "" : assetItems.SelectedItems[0].Name;
            /*System.Console.WriteLine("Tag: " + assetItems.SelectedItems[0].Tag);
            System.Console.WriteLine("ImageKey: " + assetItems.SelectedItems[0].ImageKey);
            System.Console.WriteLine("Name: " + assetItems.SelectedItems[0].Name);
            System.Console.WriteLine("Text: " + assetItems.SelectedItems[0].Text);
            System.Console.WriteLine("ToString: " + assetItems.SelectedItems[0].ToString());*/
            this.Close();
        }

        private void onCancel(object sender, EventArgs e)
        {
            this.Close();
        }

        private void onDoubleClick(object sender, MouseEventArgs e)
        {
            if (assetItems.SelectedItems.Count != 0)
            {
                m_selectedItemId = assetItems.SelectedItems[0].Name;
                this.Close();
            }
        }

    }
}