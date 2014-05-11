using System;
using System.Collections.Generic;
using System.Text;
using Net7_Tools.Database;
using System.Windows.Forms;

namespace Net7_Tools.Editor
{
    public interface IEditor
    {
        /// <summary>
        ///   <para>Activate this editor</para>
        ///   <para>Tasks include displaying the proper TabPage and displaying the "first" record</para>
        /// </summary>
        /// <returns>The id of the "main" table.</returns>
        String activate();

        /// <summary>
        /// Retrieve the id of the primary table
        /// </summary>
        /// <returns></returns>
        String getPrimaryId();

        /// <summary>
        ///   <para>Add a "page" to the editor.  Each page is composed of a TabPage,
        ///         a TableIO, and a IRecordManager</para>
        ///   <para>The first "page" is considered the "main" page, linked with the
        ///         search actions as well as containing the id to be used in the
        ///         other pages.</para>
        /// </summary>
        /// <param name="tabPage">The TabPage to add.</param>
        /// <param name="tableIO">The TableIO to add.</param>
        void addTableHandler(TabPage tabPage, TableIO tableIO);

        List<TableHandler> getTableHandlers();

        /// <summary>
        ///   <para>Create the appropriate select criteria to perform a search within the DataSet.</para>
        /// </summary>
        /// <param name="currentId">The currently selected id</param>
        /// <example>String selectCommand = "id = " + currentId;</example>
        String getRecordSelectCriteria(String currentId);

        /// <summary>
        ///   <para>Select the record corresponding to the specified id.</para>
        /// </summary>
        /// <param name="currentId">The currently selected id</param>
        Boolean selectRecord(String currentId);

        /// <summary>
        ///   <para>Use the search dialog to retrieve an id.</para>
        /// </summary>
        /// <param name="currentId">The currently selected id</param>
        /// <returns>
        ///     <para>The newly selected id</para>
        /// </returns>
        /// <remarks>Use the currentId to position the cursor, the initial selection.</remarks>
        String getIdFromSearchDialog(String currentId);

        TableIO getTableIO(TabPage tabPage);

        void addRecord();
        void deleteRecord();
        void saveRecord();
        void cancelRecord();

        void ensureState(TableIO.State state);
    }
}
