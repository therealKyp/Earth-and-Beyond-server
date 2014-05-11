Tab pages flicker too much when changing category/subcategory


//****************************************************************************

Procedure to add a new "combination", such as projectile weapons

1) frmMain.cs: enum EditorType
2) Items.cs: getEditorType()
3) Create a new RecordManager
4) DB.cs: create a new query
5) frmMain.cs: frmItems() and a new initialize...()
6) frmMain.cs: initializeItems() set the tabs to show

//****************************************************************************

Recommended Tags for Documentation Comments (C# Programming Guide)
http://msdn.microsoft.com/en-us/library/5ast78ax.aspx

C# and XML Source Code Documentation
http://www.codeproject.com/KB/XML/csharpcodedocumentation.aspx

NDoc Code Documentation Generator for .NET
http://ndoc.sourceforge.net/


//****************************************************************************
/* Documentation template */
/// <summary>
///   <para></para>
/// </summary>
/// <returns></returns>
/// <param name=""></param>
/// <exception cref=""></exception>
/// <remarks></remarks>


//****************************************************************************
/// <summary>
/// Creates a new array of arbitrary type <typeparamref name="T"/>
/// </summary>
/// <typeparam name="T">The element type of the array</typeparam>
public static T[] mkArray<T>(int n)


//****************************************************************************
<list type="bullet" | "number" | "table">
    <listheader>
        <term>term</term>
        <description>description</description>
    </listheader>
    <item>
        <term>term</term>
        <description>description</description>
    </item>
</list>
