#include "PrismPrecompiledHeader.h"
#include "Prism/Utilities/PlatformUtilities.h"
#include <commdlg.h>
#include <Glfw/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h> //Allows us to retrieve the Window handle.
#include "Prism/Core/Application.h"

namespace Prism
{
	std::optional<std::string> FileDialogs::OpenFile(const char* filter)
	{
		OPENFILENAMEA fileDialog; //Passes data to and from GetOpenFileName & GetSaveFileName. It stores settings used to create the dialog box and the results of the user's selection. 
		CHAR szFile[260] = { 0 }; //Our selected file path's buffer.
		ZeroMemory(&fileDialog, sizeof(OPENFILENAME)); //Initialize openedFile's memory to 0.
		
		fileDialog.lStructSize = sizeof(OPENFILENAME); //Sets the struct size. We do this for every Win32 struct.
		fileDialog.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::GetApplication().GetWindow().GetNativeWindow()); //Gets our currently open window and retrieves it HWND which we set as the struct's owner.
		fileDialog.lpstrFile = szFile; //Buffer for our file.
		fileDialog.nMaxFile = sizeof(szFile); //Size of our file buffer.
		fileDialog.lpstrFilter = filter; //File filter.
		fileDialog.nFilterIndex = 1; //Which filter is set by default. 
		fileDialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR; //The last flag is very important. If you don't do this and call OpenFileName, it will change the working directory for your application to the folder you open the window from.  
		
		if (GetOpenFileNameA(&fileDialog) == true)
		{
			return fileDialog.lpstrFile; //We return the file path of the file we open and create a string out of the char* path.
		}
		
		return std::nullopt; //Return empty string if no file is selected. It means the dialog has been cancelled.
	}

	std::optional<std::string> FileDialogs::SaveFile(const char* filter)
	{
		OPENFILENAMEA fileDialog;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&fileDialog, sizeof(OPENFILENAME));
		
		fileDialog.lStructSize = sizeof(OPENFILENAME);
		fileDialog.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::GetApplication().GetWindow().GetNativeWindow());
		fileDialog.lpstrFile = szFile;
		fileDialog.nMaxFile = sizeof(szFile);
		fileDialog.lpstrFilter = filter;
		fileDialog.nFilterIndex = 1;
		fileDialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA(&fileDialog) == TRUE)
		{
			return fileDialog.lpstrFile;
		}

		return std::nullopt;
	}
}