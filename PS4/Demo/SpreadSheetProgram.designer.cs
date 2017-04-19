﻿namespace SS
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.spreadsheetPanel1 = new SS.SpreadsheetPanel();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.menuFile = new System.Windows.Forms.ToolStripMenuItem();
            this.menuNew = new System.Windows.Forms.ToolStripMenuItem();
            this.menuOpen = new System.Windows.Forms.ToolStripMenuItem();
            this.menuSave = new System.Windows.Forms.ToolStripMenuItem();
            this.menuSaveAs = new System.Windows.Forms.ToolStripMenuItem();
            this.menuClose = new System.Windows.Forms.ToolStripMenuItem();
            this.menuHelp = new System.Windows.Forms.ToolStripMenuItem();
            this.extrasToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.findToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.clearSpreadSheetToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.graphToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.boxCellContents = new System.Windows.Forms.TextBox();
            this.boxCellValue = new System.Windows.Forms.TextBox();
            this.boxCurrentCell = new System.Windows.Forms.TextBox();
            this.SaveAsDialog = new System.Windows.Forms.SaveFileDialog();
            this.buttonSetContents = new System.Windows.Forms.Button();
            this.OpenFile = new System.Windows.Forms.OpenFileDialog();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // spreadsheetPanel1
            // 
            this.spreadsheetPanel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.spreadsheetPanel1.Location = new System.Drawing.Point(0, 90);
            this.spreadsheetPanel1.Margin = new System.Windows.Forms.Padding(6, 6, 6, 6);
            this.spreadsheetPanel1.Name = "spreadsheetPanel1";
            this.spreadsheetPanel1.Size = new System.Drawing.Size(1066, 638);
            this.spreadsheetPanel1.TabIndex = 0;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.menuStrip1.Dock = System.Windows.Forms.DockStyle.None;
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(40, 40);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuFile,
            this.menuHelp,
            this.extrasToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Padding = new System.Windows.Forms.Padding(12, 4, 0, 4);
            this.menuStrip1.Size = new System.Drawing.Size(276, 46);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // menuFile
            // 
            this.menuFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuNew,
            this.menuOpen,
            this.menuSave,
            this.menuSaveAs,
            this.menuClose});
            this.menuFile.Name = "menuFile";
            this.menuFile.Size = new System.Drawing.Size(64, 38);
            this.menuFile.Text = "File";
            // 
            // menuNew
            // 
            this.menuNew.Name = "menuNew";
            this.menuNew.Size = new System.Drawing.Size(212, 38);
            this.menuNew.Text = "New";
            this.menuNew.Click += new System.EventHandler(this.menuNew_Click);
            // 
            // menuOpen
            // 
            this.menuOpen.Name = "menuOpen";
            this.menuOpen.Size = new System.Drawing.Size(212, 38);
            this.menuOpen.Text = "Open";
            this.menuOpen.Click += new System.EventHandler(this.menuOpen_Click);
            // 
            // menuSave
            // 
            this.menuSave.Name = "menuSave";
            this.menuSave.Size = new System.Drawing.Size(212, 38);
            this.menuSave.Text = "Save";
            this.menuSave.Click += new System.EventHandler(this.menuSave_Click);
            // 
            // menuSaveAs
            // 
            this.menuSaveAs.Name = "menuSaveAs";
            this.menuSaveAs.Size = new System.Drawing.Size(212, 38);
            this.menuSaveAs.Text = "Save As...";
            this.menuSaveAs.Click += new System.EventHandler(this.menuSaveAs_Click);
            // 
            // menuClose
            // 
            this.menuClose.Name = "menuClose";
            this.menuClose.Size = new System.Drawing.Size(212, 38);
            this.menuClose.Text = "Close";
            this.menuClose.Click += new System.EventHandler(this.menuClose_Click);
            // 
            // menuHelp
            // 
            this.menuHelp.Name = "menuHelp";
            this.menuHelp.Size = new System.Drawing.Size(77, 38);
            this.menuHelp.Text = "Help";
            this.menuHelp.Click += new System.EventHandler(this.menuHelp_Click);
            // 
            // extrasToolStripMenuItem
            // 
            this.extrasToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.findToolStripMenuItem1,
            this.clearSpreadSheetToolStripMenuItem,
            this.graphToolStripMenuItem});
            this.extrasToolStripMenuItem.Name = "extrasToolStripMenuItem";
            this.extrasToolStripMenuItem.Size = new System.Drawing.Size(88, 38);
            this.extrasToolStripMenuItem.Text = "Extras";
            // 
            // findToolStripMenuItem1
            // 
            this.findToolStripMenuItem1.Name = "findToolStripMenuItem1";
            this.findToolStripMenuItem1.Size = new System.Drawing.Size(311, 38);
            this.findToolStripMenuItem1.Text = "Find";
            this.findToolStripMenuItem1.Click += new System.EventHandler(this.findToolStripMenuItem_Click);
            // 
            // clearSpreadSheetToolStripMenuItem
            // 
            this.clearSpreadSheetToolStripMenuItem.Name = "clearSpreadSheetToolStripMenuItem";
            this.clearSpreadSheetToolStripMenuItem.Size = new System.Drawing.Size(311, 38);
            this.clearSpreadSheetToolStripMenuItem.Text = "Clear SpreadSheet";
            this.clearSpreadSheetToolStripMenuItem.Click += new System.EventHandler(this.clearSpreadSheetToolStripMenuItem_Click);
            // 
            // graphToolStripMenuItem
            // 
            this.graphToolStripMenuItem.Name = "graphToolStripMenuItem";
            this.graphToolStripMenuItem.Size = new System.Drawing.Size(311, 38);
            this.graphToolStripMenuItem.Text = "Graph";
            this.graphToolStripMenuItem.Click += new System.EventHandler(this.graphToolStripMenuItem_Click);
            // 
            // boxCellContents
            // 
            this.boxCellContents.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.boxCellContents.Location = new System.Drawing.Point(378, 40);
            this.boxCellContents.Margin = new System.Windows.Forms.Padding(6, 6, 6, 6);
            this.boxCellContents.Name = "boxCellContents";
            this.boxCellContents.Size = new System.Drawing.Size(424, 31);
            this.boxCellContents.TabIndex = 2;
            // 
            // boxCellValue
            // 
            this.boxCellValue.Location = new System.Drawing.Point(218, 40);
            this.boxCellValue.Margin = new System.Windows.Forms.Padding(6, 6, 6, 6);
            this.boxCellValue.Name = "boxCellValue";
            this.boxCellValue.ReadOnly = true;
            this.boxCellValue.Size = new System.Drawing.Size(144, 31);
            this.boxCellValue.TabIndex = 3;
            // 
            // boxCurrentCell
            // 
            this.boxCurrentCell.Location = new System.Drawing.Point(6, 40);
            this.boxCurrentCell.Margin = new System.Windows.Forms.Padding(6, 6, 6, 6);
            this.boxCurrentCell.Name = "boxCurrentCell";
            this.boxCurrentCell.ReadOnly = true;
            this.boxCurrentCell.Size = new System.Drawing.Size(196, 31);
            this.boxCurrentCell.TabIndex = 4;
            // 
            // SaveAsDialog
            // 
            this.SaveAsDialog.DefaultExt = "sprd";
            this.SaveAsDialog.Filter = "Spreadsheet (*.sprd) |*.sprd| All Files (*.*)|*.*";
            this.SaveAsDialog.Title = "Save Spreadsheet As...";
            this.SaveAsDialog.FileOk += new System.ComponentModel.CancelEventHandler(this.SaveAsDialog_FileOk);
            // 
            // buttonSetContents
            // 
            this.buttonSetContents.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonSetContents.Location = new System.Drawing.Point(822, 40);
            this.buttonSetContents.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.buttonSetContents.Name = "buttonSetContents";
            this.buttonSetContents.Size = new System.Drawing.Size(198, 38);
            this.buttonSetContents.TabIndex = 5;
            this.buttonSetContents.Text = "Set Cell Contents";
            this.buttonSetContents.UseVisualStyleBackColor = true;
            this.buttonSetContents.Click += new System.EventHandler(this.buttonSetContents_Click);
            // 
            // OpenFile
            // 
            this.OpenFile.DefaultExt = "sprd";
            this.OpenFile.Filter = "Spreadsheet (*.sprd) |*.sprd| All Files (*.*)|*.*";
            this.OpenFile.FileOk += new System.ComponentModel.CancelEventHandler(this.OpenFile_FileOk);
            // 
            // Form1
            // 
            this.AcceptButton = this.buttonSetContents;
            this.AutoScaleDimensions = new System.Drawing.SizeF(12F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1066, 731);
            this.Controls.Add(this.buttonSetContents);
            this.Controls.Add(this.boxCurrentCell);
            this.Controls.Add(this.boxCellValue);
            this.Controls.Add(this.boxCellContents);
            this.Controls.Add(this.spreadsheetPanel1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Margin = new System.Windows.Forms.Padding(6, 6, 6, 6);
            this.MinimumSize = new System.Drawing.Size(934, 579);
            this.Name = "Form1";
            this.Text = "New Spreadsheet.sprd*";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private SpreadsheetPanel spreadsheetPanel1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem menuFile;
        private System.Windows.Forms.ToolStripMenuItem menuNew;
        private System.Windows.Forms.ToolStripMenuItem menuClose;
        private System.Windows.Forms.TextBox boxCellContents;
        private System.Windows.Forms.ToolStripMenuItem menuHelp;
        private System.Windows.Forms.TextBox boxCellValue;
        private System.Windows.Forms.TextBox boxCurrentCell;
        private System.Windows.Forms.ToolStripMenuItem menuSave;
        private System.Windows.Forms.ToolStripMenuItem menuSaveAs;
        private System.Windows.Forms.ToolStripMenuItem menuOpen;
        private System.Windows.Forms.SaveFileDialog SaveAsDialog;
        private System.Windows.Forms.Button buttonSetContents;
        private System.Windows.Forms.OpenFileDialog OpenFile;
        private System.Windows.Forms.ToolStripMenuItem extrasToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem findToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem clearSpreadSheetToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem graphToolStripMenuItem;
    }
}
