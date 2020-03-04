using SharkCompiler.Clang;
using System;
using System.Collections.Generic;

namespace EVClassLib
{
    [Serializable]
    public class Data
    {
        /// <summary>
        /// 数据的键值，不能重复
        /// </summary>
        public string Key { get; set; }

        /// <summary>
        /// 数据的值
        /// </summary>
        public object Value { get; set; }

        public Data() { }

        public Data(string key) : this()
        {
            this.Key = key;
        }

        public Data(string key, object value) : this(key)
        {
            this.Value = value;
        }
    }

    /// <summary>
    /// 数据库
    /// </summary>
    [Serializable]
    public class DataBase : ITransform
    {
        public string Name { get; set; }

        public List<Data> Datas { get; set; }

        [NonSerialized]
        private ClangData _clangData;

        public DataBase()
        {
            this.Name = "default";
            this.Datas = new List<Data>();
        }

        public DataBase(string name) : this()
        {
            this.Name = name;
        }

        public string TransformToClang_forC()
        {
            _clangData = new ClangData();
            ClangFunctionDefintion fundef = new ClangFunctionDefintion("Data_Init", "void", new List<ClangVariable>());
            ClangFunctionImplementation funimp = new ClangFunctionImplementation(fundef);
            List<ClangStatement> statements = new List<ClangStatement>();
            foreach (Data d in Datas)
            {
                ClangVariable v = new ClangVariable("Data_" + d.Key, GetObjectType(d.Value), ClangVariableKind.Global, d.Value);
                ClangAssignmentStatement assStmt = new ClangAssignmentStatement(new ClangNameExpression("Data_" + d.Key), new ClangValueExpression(d.Value));
                statements.Add(assStmt);
                if (d.Value.GetType().Name == "String")
                {
                    v.IsPointer = true;
                }
                _clangData.Variables.Add(v);
            }
            funimp.Body = new ClangProcedure(statements);
            _clangData.FunctionDefs.Add(fundef);
            _clangData.FunctionImps = new List<ClangFunctionImplementation>();
            _clangData.FunctionImps.Add(funimp);
            return _clangData.CreateCode_forC();
        }

        private string GetObjectType(object obj)
        {
            switch (obj.GetType().Name)
            {
                case "Int32":
                    return "uint32_t";
                case "String":
                    return "char";
                case "Double":
                    return "double";
                case "Float":
                    return "float";
                default:
                    return "void";
            }
        }

        public string TransformToClang_forH()
        {
            if (_clangData != null)
            {
                return _clangData.CreateCode_forH();
            }
            return "";
        }
    }
}
