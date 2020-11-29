#include "AssetsWidget.h"
#include "../Utilities/FileDialog.h"

namespace AssetsStatics
{
	static bool g_ShowFileDialogView = true;
	static bool g_ShowFileDialogLoad = false;
	static std::string g_DoubleClickedPathImportDialog;
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
			AssetsStatics::g_ShowFileDialogLoad = true;
		}

		ImGui::SameLine();

		// VIEW
		m_FileDialogView->Show(&AssetsStatics::g_ShowFileDialogView);

		ImGui::End();
	}

	void AssetsWidget::OnPathClicked(const std::string& path) const
	{

	}
}
