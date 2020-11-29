#pragma once
#include "Prism.h"

namespace Prism
{
	class FileDialog;

	class AssetsWidget
	{
	public:
		AssetsWidget();
		void OnAssetsWidgetUpdate();

	private:
		void OnPathClicked(const std::string& path) const;

		Scope<FileDialog> m_FileDialogView;
		Scope<FileDialog> m_FileDialogLoad;
	};
}