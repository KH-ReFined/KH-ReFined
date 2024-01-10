using System;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using System.ComponentModel;
using System.Threading.Tasks;
using System.Collections.Generic;


namespace ReFined.Forms
{
    partial class FormDL
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

        private void InitializeComponent()
        {
            this.dlProgress = new System.Windows.Forms.ProgressBar();
            this.buttonAbort = new System.Windows.Forms.Button();
            this.labelDesc = new System.Windows.Forms.Label();
            this.SuspendLayout();

            this.dlProgress.Location = new System.Drawing.Point(16, 37);
            this.dlProgress.Name = "dlProgress";
            this.dlProgress.Size = new System.Drawing.Size(286, 23);
            this.dlProgress.TabIndex = 0;

            this.buttonAbort.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonAbort.Location = new System.Drawing.Point(16, 67);
            this.buttonAbort.Name = "buttonAbort";
            this.buttonAbort.Size = new System.Drawing.Size(286, 40);
            this.buttonAbort.TabIndex = 1;
            this.buttonAbort.Text = "Abort...";
            this.buttonAbort.UseVisualStyleBackColor = true;
            this.buttonAbort.Click += new System.EventHandler(this.OnTriggerAbort);

            this.labelDesc.AutoSize = true;
            this.labelDesc.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelDesc.Location = new System.Drawing.Point(12, 9);
            this.labelDesc.Name = "labelDesc";
            this.labelDesc.Size = new System.Drawing.Size(290, 21);
            this.labelDesc.TabIndex = 2;
            this.labelDesc.Text = "Downloading Update... Please be patient.";
            this.labelDesc.TextAlign = System.Drawing.ContentAlignment.TopCenter;

            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(317, 116);
            this.Controls.Add(this.labelDesc);
            this.Controls.Add(this.buttonAbort);
            this.Controls.Add(this.dlProgress);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "FormDL";
            this.Text = "Downloading...";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        public System.Windows.Forms.ProgressBar dlProgress;
        private System.Windows.Forms.Button buttonAbort;
        private System.Windows.Forms.Label labelDesc;
    }

    public partial class FormDL : Form
    {
        public FormDL()
        {
            InitializeComponent();
        }

        private void OnTriggerAbort(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Abort;
			this.Close();
        }

		public void CompleteCall()
		{
			this.DialogResult = DialogResult.OK;
			this.Close();
		}
    }
}
