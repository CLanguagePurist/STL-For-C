using System;
using System.IO;
using System.Text.RegularExpressions;
namespace Workflows{
    static class Program{
        static async Task<int> Main(string[] args)
        {
            string projectRoot = Environment.CurrentDirectory;
            if (string.IsNullOrWhiteSpace(projectRoot))
                return 1;
            if (!projectRoot.Contains("STL-For-C")) return 1;
            if (projectRoot.Contains(".github/workflows"))
                projectRoot = Directory.GetParent(Directory.GetParent(projectRoot).FullName).FullName;

            var logContent = await File.ReadAllLinesAsync(projectRoot);

            bool PASS = true;

            foreach (var line in logContent)
            {
                var trimmedLine = line.TrimStart();
                if (trimmedLine.StartsWith("Fail:") || 
                    trimmedLine.StartsWith("Unexpected Pass:") ||
                    trimmedLine.StartsWith("Timeout:"))
                {
                    var items = trimmedLine.Split(':', StringSplitOptions.TrimEntries);
                    long count;
                    if (!long.TryParse(items.Last(), out count))
                        continue;
                    if (count > 0)
                        PASS = false;
                }
            }
            Console.WriteLine(PASS ? "::set-output name=status::'PASS'" : "::set-output name=status::'FAIL'");

            return 0;
        }
    }
}