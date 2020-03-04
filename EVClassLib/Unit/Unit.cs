using SharkCompiler;
using SharkCompiler.Clang;
using System;
using System.Collections.Generic;

namespace EVClassLib
{
    [Serializable]
    public class Unit : ITransform
    {
        public string Name { set; get; }

        public ExecModule Parent { get; set; }

        public UnitType Type { get; set; }

        public bool Enabled { get; set; }

        [NonSerialized]
        public ClangTempData Data;

        public Dictionary<int, string> Setting { set; get; }

        public Unit(string name, UnitType type, bool enabled) : this(name)
        {
            this.Type = type;
            this.Enabled = enabled;
        }

        public Unit(string name)
        {
            this.Name = name;
        }

        public virtual string TransformToClang_forC()
        {
            string text = "#include \"GLOBAL.h\"\r\n";
            if (Data != null)
            {
                ClangFunctionImplementation imp = Data.FunctionImps.Find(p => p.Reference.Name == Data.Name + "_Init");
                ClangFunctionDefintion def;
                if (imp == null)
                {
                    def = new ClangFunctionDefintion(Data.Name + "_Init", "void", new List<ClangVariable>());
                    imp = new ClangFunctionImplementation(def);
                    imp.Body = new ClangProcedure(Data.Statements);
                    Data.FunctionDefs.Add(def);
                    Data.FunctionImps.Add(imp);
                }
                else
                {
                    imp.Body.Statements.InsertRange(0, Data.Statements);
                }
                text += Data.ToClangData().CreateCode_forC();
            }
            return text;
        }

        public virtual string TransformToClang_forH()
        {
            string text = "";
            if (Data != null)
            {
                text += Data.ToClangData().CreateCode_forH();
            }
            return text;
        }
    }
}
