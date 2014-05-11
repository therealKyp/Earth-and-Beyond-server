using System;
using System.Collections.Generic;
using System.Text;
using System.Data;

namespace Net7_Tools.Database
{
    public interface IRecordManager
    {
        /// <summary>
        ///   <para>
        ///     Display the value within the appropriate DataRow column into
        ///     the field contained within DataBinding</para>
        /// </summary>
        /// <returns>
        ///     <para>True: the field has been displayed.</para>
        ///     <para>False: the field has not been displayed, use the default code </para>
        /// </returns>
        Boolean displayField(DataBinding dataBinding, DataRow dataRow);

        /// <summary>
        ///   <para>
        ///     Enable the field contained within DataBinding</para>
        /// </summary>
        /// <returns>
        ///     <para>True: the field has been enabled.</para>
        ///     <para>False: the field has not been enabled, use the default code </para>
        /// </returns>
        Boolean enableField(DataBinding dataBinding, DataRow dataRow, Boolean enabled);

        /// <summary>
        ///   <para>
        ///     Validate the display the value of the field contained within DataBinding</para>
        /// </summary>
        /// <returns>
        ///     <para>The data validation result.</para>
        /// </returns>
        DataValidation.Result validField(Boolean newRecord, DataBinding dataBinding, DataRow dataRow);

        /// <summary>
        ///   <para>
        ///     Update the value within the appropriate DataRow column from
        ///     the contents of the field within DataBinding</para>
        /// </summary>
        /// <returns>
        ///     <para>True: the data has been updated.</para>
        ///     <para>False: the data has not been updated, use the default code </para>
        /// </returns>
        Boolean updateField(Boolean newRecord, DataBinding dataBinding, DataRow dataRow);

        /// <summary>
        ///   <para>
        ///     Compose a select criteria on the specified id.  This criteria
        ///     will be used to locate the relevant DataRow within the DataSet.
        /// </para>
        /// </summary>
        /// <returns>
        ///     <para>The appropriate select critera.</para>
        /// </returns>
        /// <example>return "id = " + currentId;</example>
        String selectDataRowFromId(String currentId);
    }
}
