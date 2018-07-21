#include <obs-module.h>
#include <obs-frontend-api.h>

#include <QAction>
#include <QMessageBox>

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("obs-screenshot", "en-US")

void HandleScreenshot()
{
	QMessageBox::information(nullptr, "foo", "foo", QMessageBox::Ok);
}

bool obs_module_load(void)
{
	QAction *action = (QAction*)obs_frontend_add_tools_menu_qaction(
		obs_module_text("Screenshot.TakeScreenshot"));
	action->connect(action, &QAction::triggered, &HandleScreenshot);

	return true;
}

void obs_module_unload(void)
{
}

