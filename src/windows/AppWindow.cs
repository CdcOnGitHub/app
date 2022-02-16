using System;
using System.Windows;
using System.Reflection;

namespace GeodeApp {
    abstract class AppWindow : Window {
        public AppWindow(string title, int width, int height) {
            this.Title = $"{title} • ({Assembly.GetExecutingAssembly().GetName().Version.ToString()})";
            this.Width = width;
            this.Height = height;
            this.WindowStartupLocation = WindowStartupLocation.CenterScreen;
            this.Background = Theme.Colors.Background;
        }
    }
}
