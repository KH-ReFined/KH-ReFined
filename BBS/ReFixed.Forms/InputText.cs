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
            this.inputBox = new System.Windows.Forms.TextBox();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.buttonAnsem = new System.Windows.Forms.Button();
            this.SuspendLayout();

            this.labelTask.AutoSize = true;
            this.labelTask.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelTask.Location = new System.Drawing.Point(12, 9);
            this.labelTask.Name = "labelTask";
            this.labelTask.Size = new System.Drawing.Size(354, 21);
            this.labelTask.TabIndex = 0;
            this.labelTask.Text = "Please type in the name you want for this finisher.";

            this.inputBox.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.inputBox.Location = new System.Drawing.Point(16, 42);
            this.inputBox.MaxLength = 16;
            this.inputBox.Name = "inputBox";
            this.inputBox.Size = new System.Drawing.Size(350, 29);
            this.inputBox.TabIndex = 2;
            this.inputBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.inputBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.eventKeyDown);

            this.buttonCancel.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonCancel.Location = new System.Drawing.Point(194, 81);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(172, 35);
            this.buttonCancel.TabIndex = 3;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.eventCancel);

            this.buttonAnsem.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonAnsem.Location = new System.Drawing.Point(16, 81);
            this.buttonAnsem.Name = "buttonAnsem";
            this.buttonAnsem.Size = new System.Drawing.Size(172, 35);
            this.buttonAnsem.TabIndex = 4;
            this.buttonAnsem.Text = "Submit";
            this.buttonAnsem.UseVisualStyleBackColor = true;
            this.buttonAnsem.Click += new System.EventHandler(this.eventSubmit);

            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(385, 127);
            this.Controls.Add(this.buttonAnsem);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.inputBox);
            this.Controls.Add(this.labelTask);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "InputText";
            this.Text = "Rename Finisher";
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.eventKeyDown);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Label labelTask;
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
