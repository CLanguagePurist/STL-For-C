using System;
using System.IO;
using System.Text.RegularExpressions;
namespace Workflows{
    static class Program{
        static int Main(string[] args)
        {
            string projectRoot = Environment.CurrentDirectory;
            if (string.IsNullOrWhiteSpace(projectRoot))
                return 1;
            if (!projectRoot.Contains("STL-For-C")) return 1;
            if (projectRoot.Contains(".github/workflows"))
                projectRoot = Directory.GetParent(Directory.GetParent(projectRoot).FullName).FullName;

            var logContent = File.ReadAllText(projectRoot);

            // TODO: 

            return 0;
        }
    }
}