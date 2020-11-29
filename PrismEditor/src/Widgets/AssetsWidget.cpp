#include "AssetsWidget.h"
#include "FileDialog.h"

namespace Widget_Assets_Statics
{
	static bool g_show_file_dialog_view = true;
	static bool g_show_file_dialog_load = false;
	static std::string g_double_clicked_path_import_dialog;
}

namespace Prism
{
	AssetsWidget::AssetsWidget()
	{
		m_FileDialogView = CreateScope<FileDialog>(false, FileDialogType_Browser, FileDialogOperation_Load, FileDialogFilter_All);
		m_FileDialogLoad = CreateScope<FileDialog>(true, FileDialogType_FileSelection, FileDialogOperation_Load, FileDialogFilter_Model);
	}

	void AssetsWidget::OnAssetsWidgetUpdate()
	{
		ImGui::Begin("Assets");

		if (ImGui::Button("Import"))
		{
			Widget_Assets_Statics::g_show_file_dialog_load = true;
		}

		ImGui::SameLine();

		// VIEW
		m_FileDialogView->Show(&Widget_Assets_Statics::g_show_file_dialog_view);

		ImGui::End();
	}

	void AssetsWidget::OnPathClicked(const std::string& path) const
	{

	}
}
