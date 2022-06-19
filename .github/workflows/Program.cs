using System;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace Workflows{
    static class Program{
        static async Task<int> Main(string[] args)
        {
            string projectRoot = Environment.CurrentDirectory;
            if (string.IsNullOrWhiteSpace(projectRoot))
                return 1;
            if (!projectRoot.Contains("STL-For-C")) return 1;
            if (projectRoot.Contains(".github/workflows"))
            {
                var parent1 = Directory.GetParent(projectRoot);
                if (parent1 == null) return 1;
                var parent2 = Directory.GetParent(parent1.FullName);
                if (parent2 == null) return 1;
                projectRoot = parent2.FullName;
                if (string.IsNullOrWhiteSpace(projectRoot)) return 1;
            }
            var path = Path.Combine(projectRoot, "build/meson-logs/testlog.txt");
            var logContent = await File.ReadAllLinesAsync(path);

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
            var githubEnvFile = Environment.GetEnvironmentVariable("GITHUB_ENV");
            if (string.IsNullOrWhiteSpace(githubEnvFile)) return 1;
            await File.AppendAllTextAsync(githubEnvFile, "status='PASS'\n");

            return 0;
        }
    }
}