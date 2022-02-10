/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR BBS!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ReFixed.Forms
{
    partial class InputText
    {
        private System.ComponentModel.IContainer components = null;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

            private void InitializeComponent()
            {
                this.labelTask = new System.Windows.Forms.Label();
                this.labelInfo = new System.Windows.Forms.Label();
                this.inputBox = new System.Windows.Forms.TextBox();
                this.buttonCancel = new System.Windows.Forms.Button();
                this.buttonAnsem = new System.Windows.Forms.Button();
                this.SuspendLayout();

                this.labelTask.AutoSize = true;
                this.labelTask.Location = new System.Drawing.Point(12, 9);
                this.labelTask.Name = "labelTask";
                this.labelTask.Size = new System.Drawing.Size(269, 15);
                this.labelTask.TabIndex = 0;
                this.labelTask.Text = "Please type in the name you want for this finisher.";

                this.labelInfo.AutoSize = true;
                this.labelInfo.Location = new System.Drawing.Point(22, 26);
                this.labelInfo.Name = "labelInfo";
                this.labelInfo.Size = new System.Drawing.Size(246, 15);
                this.labelInfo.TabIndex = 1;
                this.labelInfo.Text = "(A maximum of 16 characters are supported.)";

                this.inputBox.Location = new System.Drawing.Point(12, 50);
                this.inputBox.MaxLength = 16;
                this.inputBox.Name = "inputBox";
                this.inputBox.Size = new System.Drawing.Size(269, 23);
                this.inputBox.TabIndex = 2;

                this.buttonCancel.Location = new System.Drawing.Point(150, 80);
                this.buttonCancel.Name = "buttonCancel";
                this.buttonCancel.Size = new System.Drawing.Size(131, 23);
                this.buttonCancel.TabIndex = 3;
                this.buttonCancel.Text = "Cancel";
                this.buttonCancel.UseVisualStyleBackColor = true;
                this.buttonCancel.Click += new System.EventHandler(this.eventCancel);

                this.buttonAnsem.Location = new System.Drawing.Point(12, 80);
                this.buttonAnsem.Name = "buttonAnsem";
                this.buttonAnsem.Size = new System.Drawing.Size(131, 23);
                this.buttonAnsem.TabIndex = 4;
                this.buttonAnsem.Text = "Submit";
                this.buttonAnsem.UseVisualStyleBackColor = true;
                this.buttonAnsem.Click += new System.EventHandler(this.eventSubmit);

                this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
                this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
                this.ClientSize = new System.Drawing.Size(293, 112);
                this.Controls.Add(this.buttonAnsem);
                this.Controls.Add(this.buttonCancel);
                this.Controls.Add(this.inputBox);
                this.Controls.Add(this.labelInfo);
                this.Controls.Add(this.labelTask);
                this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
                this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.eventKeyDown);
                this.Name = "InputText";
                this.Text = "Rename Finisher";
                this.ResumeLayout(false);
                this.PerformLayout();

            }

        #endregion

        private Label labelTask;
        private Label labelInfo;
        private TextBox inputBox;
        private Button buttonCancel;
        private Button buttonAnsem;
    }

    public partial class InputText : Form
    {
        public InputText()
        {
            InitializeComponent();
        }

        public string FinisherName
        {
            get { return inputBox.Text; }
        }

        private void eventSubmit(object sender, EventArgs e)
        {
            if (FinisherName.Length > 0)
            {
                DialogResult = DialogResult.OK;
                Close();
            }

            else
            {
                DialogResult = DialogResult.Cancel;
                Close();
            }
        }

        private void eventCancel(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            Close();
        }

        private void eventKeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter && FinisherName.Length > 0x00)
            {
                DialogResult = DialogResult.OK;
                Close();
            }
        }
    }
}
