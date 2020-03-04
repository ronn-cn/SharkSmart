using SharkCompiler;
using SharkCompiler.Clang;
using System;
using System.Collections.Generic;

namespace EVClassLib
{
    public enum PlanType
    {
        /// <summary>
        /// 持续执行
        /// </summary>
        Continue = 0,
        /// <summary>
        /// 单次执行
        /// </summary>
        Single,
    }

    public enum EnabledType
    {
        /// <summary>
        /// 自动
        /// </summary>
        Auto = 0,
        /// <summary>
        /// 手动
        /// </summary>
        Manual,
    }
    [Serializable]
    public class Plan : ITransform
    {
        //public Tiggle
        public string Name { set; get; }

        public int Priority { set; get; }

        public int Size { set; get; }

        public int TimeSpan { set; get; }

        public int TimeStart { set; get; }

        public int TimeEnd { set; get; }

        public PlanType PlanType { get; set; }

        public EnabledType Enabled { get; set; }

        [NonSerialized]
        public ClangTempData Data;

        public Dictionary<int, string> Setting { set; get; }

        public Plan()
        {
            this.Size = 128;
            this.TimeSpan = 100;
            this.TimeStart = 0;
            this.TimeEnd = -1;
            this.PlanType = PlanType.Continue;
            this.Enabled = EnabledType.Auto;
        }

        public virtual string TransformToClang_forC()
        {
            string text = @"#include ""GLOBAL.h""
uint32_t*	TASK" + this.Name + @"_STK;
void		Task" + this.Name + @"(void *p_arg);";
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
                ClangFunctionImplementation imp2 = Data.FunctionImps.Find(p => p.Reference.Name == Data.Name + "_Loop");
                text += @"void Task" + this.Name + @"(void *p_arg)
{
    while(1)
	{
		";
                if (imp2 != null) { text += imp2.Reference.Name + "();"; }
                text += @"
		OSTimeDly(100);
";
                if (this.PlanType == PlanType.Single)
                {
                    text += @"OSTaskSuspend(USER_TASK_PRIO_NUM + " + this.Priority + @");
break;";
                }
                text += @"
	}
}

void " + this.Name + @"_Start()
{
    TASK" + this.Name + @"_STK = mymalloc(SRAMEX, " + this.Size.ToString() + @" * 4);
    OSTaskCreate(Task" + this.Name + @",""Task" + this.Name + @""", (void*)0, (uint32_t*)&TASK" + this.Name + @"_STK[" + (this.Size - 1).ToString() + @"], USER_TASK_PRIO_NUM + " + this.Priority + @");
}

void " + this.Name + @"_Stop()
{
    OSTaskSuspend(USER_TASK_PRIO_NUM + " + this.Priority + @");
}
";
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
            text += "extern void " + this.Name + "_Start();\r\n";
            text += "extern void " + this.Name + "_Stop();\r\n";
            return text;
        }
    }
}
