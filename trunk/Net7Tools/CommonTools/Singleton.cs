using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace Singleton
{
    /// <summary>
    /// Inherit a singleton:
    /// <example>
    /// <para>public class MyClass : Singleton.Get&lt;MyClass></para>
    /// <para>MyClass myClass = MyClass.Instance</para>
    /// </example>
    /// </summary>
    /// <typeparam name="T"></typeparam>
    // http://www.codeproject.com/KB/cs/genericsingleton.aspx?fid=201948&df=90&mpp=25&noise=3&sort=Position&view=Quick&select=2254561#xx2254561xx
    public class Get<T> where T : new()
    {
        protected Get()
        {
            if (Instance != null)
            {
                throw (new Exception("You have tried to create a new singleton class where you should have instanced it. Replace your \"new class()\" with \"class.Instance\""));
            }
        }

        public static T Instance
        {
            get { return SingletonCreator.instance; }
        }

        class SingletonCreator
        {
            static SingletonCreator()
            {

            }
            internal static readonly T instance = new T();
        }
    }

}
