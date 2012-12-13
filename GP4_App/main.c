
/**
* @author Vijay Kanta
* @website www.vizcreations.com
* @abstract Simple console configurator for game Grand Prix 4
* @license Free software - No guarantee
* @copyright Not applicable
* @link https://github.com/vijaykanta/gp4_gfx_config.git
*/

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "resource.h"

typedef struct Configurator {
	char file_name[256];
	char *cur_line;
	char **all_lines;
	int tot_lines;
	int cur_sel;
	FILE *fp;
	int global;
	int global_key;
	char *global_cmt;
	int res_width;
	int res_width_key;
	char *res_width_cmt;
	int res_height;
	int res_height_key;
	char *res_height_cmt;
	int hw_tnl;
	int hw_tnl_key;
	char *hw_tnl_cmt;
	int wnd_mode;
	int wnd_mode_key;
	char *wnd_mode_cmt;
	int vsync;
	int vsync_key;
	char *vsync_cmt;
	int tex_qlty;
	int tex_qlty_key;
	char *tex_qlty_cmt;
	int bump_map;
	int bump_map_key;
	char *bump_map_cmt;
	int mirrors;
	int mirrors_key;
	char *mirrors_cmt;
	int track_map;
	int track_map_key;
	char *track_map_cmt;
	int env_map;
	int env_map_key;
	char *env_map_cmt;
	int tex_fil_qlty;
	int tex_fil_qlty_key;
	char *tex_fil_qlty_cmt;
	int an_fil_qlty;
	int an_fil_qlty_key;
	char *an_fil_qlty_cmt;
	int shdws;
	int shdws_key;
	char *shdws_cmt;
	int shdw_type;
	int shdw_type_key;
	char *shdw_type_cmt;
	int ext_steer;
	int ext_steer_key;
	char *ext_steer_cmt;
	int heat_haze;
	int heat_haze_key;
	char *heat_haze_cmt;
} CFG;

#define MAX_PATHE 1000
#define MAX_STRUCT 5000
//#define MAX_LEN 200
#define MAX_LINE 255
#define MAX_LINES 100
#define MAX_VAL 20

#define STRLEN 100
#define MAX_LEN 5000

const char g_szClassName[] = "myWindowClass";
HWND g_hToolBar = NULL;
HBRUSH g_hbrBackground = NULL;
HFONT hDefault;
HDC hdcStatic;
OPENFILENAME ofn;
char szFileName[MAX_PATHE] = "";
FILE *fp;
FILE *fw;
char *line;
char **lines;
int val;
int i, j, k;

char *token;
char *buff;
char *value;
char *comment;
int val;
int i;
char sel;
//char *token;
char text[15];

int texfilq = 0;
int trkmp = 0;
int envmp = 0;
int texq = 0;
int mxanfil = 1;
int shdwt = 0;

char delimiters[] = " .,;|";

void strip_newline(char *str, int len) {
	int i = 0;
	for(i=0; i<len; i++) {
		if(str[i] == '\n') {
			str[i] = '\0';
			break;
		}
	}
}

void strip_tab(char *str, int len) {
	int i = 0;
	for(i=0; i<len; i++) {
		if(str[i] == '\t') {
			str[i] = '\0';
			break;
		}
	}
}

void save_file(CFG *cfg) {
	int i;
	printf("Generating new configuration file from buffer..\n");
	cfg->fp = fopen("f1graphics.cfg", "w");
	if(!cfg->fp) {
		printf("Couldn't write file!\n");
	} else {
		for(i=0; i<cfg->tot_lines; i++) {
			fprintf(cfg->fp, "%s\n", cfg->all_lines[i]);
		}
		fclose(cfg->fp);
		printf("New configuration file created successfully!\n");
	}
}

void show_file_contents(CFG *cfg) {
	char *buff;
	int counter = 0;
	cfg->fp = fopen(cfg->file_name, "r");
	//if(sizeof(buff) > 0) free(buff);
	buff = (char *) malloc(sizeof(char) * MAX_LINE);
	while(fgets(buff, MAX_LINE, cfg->fp)) {
		printf("%d: %s", counter, buff);
		free(buff);
		buff = (char *) malloc(sizeof(char) * MAX_LINE);
		++counter;
	}
	free(buff);
}

void get_val_cmt_(CFG *cfg) {
	if(sizeof(buff) > 0) free(buff);
	if(sizeof(value) > 0) free(value);
	if(sizeof(comment) > 0) free(comment);

	buff = (char *) malloc(sizeof(char) * MAX_LINE);
	value = (char *) malloc(sizeof(char) * MAX_VAL);
	comment = (char *) malloc(sizeof(char) * MAX_LINE);
	
	strcpy(buff, cfg->all_lines[cfg->cur_sel]);
	//MessageBox(NULL, buff, "New line", MB_OK);
	token = strtok(buff, ";");

	if(token != NULL) {
		//value[0] = '\0';
		strcpy(value, token);
		strip_tab(value, MAX_VAL);
	}

	token = strtok(NULL, ";");

	if(token != NULL) {
		//comment[0] = '\0';
		strcpy(comment, token);
		strip_newline(comment, MAX_LINE);
	}

}

void set_val_cmt_(CFG *cfg) {
	int last;
	free(cfg->all_lines[cfg->cur_sel]);
	cfg->all_lines[cfg->cur_sel] = (char *) malloc(sizeof(char) * MAX_LINE);
	cfg->all_lines[cfg->cur_sel][0] = '\0';

	free(value);
	value = (char *) malloc(sizeof(char) * MAX_VAL);
	//value[0] = '\0';

	sprintf(value, "%d\t\t\t", val);
	strcat(cfg->all_lines[cfg->cur_sel], value);
	strcat(cfg->all_lines[cfg->cur_sel], ";");
	strcat(cfg->all_lines[cfg->cur_sel], comment);

	last = strlen(cfg->all_lines[cfg->cur_sel]) + 1;
	cfg->all_lines[cfg->cur_sel][last] = '\0';
}

void show_cur_val(CFG *cfg) {
	printf("Change settings as below:\n");
	printf("%s\n", comment);
}

/*void start_(CFG *cfg) {
	printf("\nType 't' to edit Texture Filter Quality\n");
	printf("Type 'm' to edit Track Map\n");
	printf("Type 'v' to edit Environment Map\n");
	printf("Type 'q' to edit Texture Quality\n");
	printf("Type 'a' to edit Anisotropic Filter Quality\n");
	printf("Type 's' to edit Shadow Type\n");
	printf("Type 'c' to generate configuration file from buffer\n");
	printf("Type 'x' to exit\n\n");

	scanf("%c", &sel);
	if(sel == 't' || sel == 'T') {
		cfg->cur_sel = cfg->tex_fil_qlty_key;
		get_val_cmt_(cfg);
		show_cur_val(cfg);

		printf("Enter value for Texture Filter Quality (%s - current): ", value);
		scanf("%d", &val);
		cfg->tex_fil_qlty = val;

		strcpy(cfg->tex_fil_qlty_cmt, comment);

		set_val_cmt_(cfg);
		printf("Value edited successfully..\n");
		getchar();
		start_(cfg);
	} else if(sel == 'm' || sel == 'M') {
		cfg->cur_sel = cfg->track_map_key;
		get_val_cmt_(cfg);
		show_cur_val(cfg);

		printf("Enter value for Track Map (%s - current): ", value);
		scanf("%d", &val);
		cfg->track_map = val;

		strcpy(cfg->track_map_cmt, comment);

		set_val_cmt_(cfg);
		printf("Value edited successfully..\n");
		getchar();
		start_(cfg);
	} else if(sel == 'v' || sel == 'V') {
		cfg->cur_sel = cfg->env_map_key;
		get_val_cmt_(cfg);
		show_cur_val(cfg);

		printf("Enter value for Environment Map (%s - current): ", value);
		scanf("%d", &val);
		cfg->env_map = val;

		strcpy(cfg->env_map_cmt, comment);

		set_val_cmt_(cfg);
		printf("Value edited successfully..\n");
		getchar();
		start_(cfg);
	} else if(sel == 'q' || sel == 'Q') {
		cfg->cur_sel = cfg->tex_qlty_key;
		get_val_cmt_(cfg);
		show_cur_val(cfg);

		printf("Enter value for Texture Quality (%s - current): ", value);
		scanf("%d", &val);
		cfg->tex_qlty = val;

		strcpy(cfg->tex_qlty_cmt, comment);

		set_val_cmt_(cfg);
		printf("Value edited successfully..\n");
		getchar();
		start_(cfg);
	} else if(sel == 'a' || sel == 'A') {
		cfg->cur_sel = cfg->an_fil_qlty_key;
		get_val_cmt_(cfg);
		show_cur_val(cfg);

		printf("Enter value for Anisotropic Filter Quality (%s - current): ", value);
		scanf("%d", &val);
		cfg->an_fil_qlty = val;

		strcpy(cfg->an_fil_qlty_cmt, comment);

		set_val_cmt_(cfg);
		printf("Value edited successfully..\n");
		getchar();
		start_(cfg);
	} else if(sel == 's' || sel == 'S') {
		cfg->cur_sel = cfg->shdw_type_key;
		get_val_cmt_(cfg);
		show_cur_val(cfg);

		printf("Enter value for Shadow Type (%s - current): ", value);
		scanf("%d", &val);
		cfg->shdw_type = val;

		set_val_cmt_(cfg);
		printf("Value edited successfully..\n");
		getchar();
		start_(cfg);
	} else if(sel == 'c' || sel == 'C') {
		save_file(cfg);
		getchar();
		start_(cfg);
	} else if(sel == 'f' || sel == 'F') {
		show_file_contents(cfg);
		getchar();
		start_(cfg);
	}
}*/

void init_vals(CFG *cfg) {
	cfg->tex_fil_qlty_key = 98;
	cfg->cur_sel = cfg->tex_fil_qlty_key;
	get_val_cmt_(cfg);
	cfg->tex_fil_qlty_cmt = (char *) malloc(sizeof(char) * MAX_LEN);
	cfg->tex_fil_qlty = (int) value;
	strcpy(cfg->tex_fil_qlty_cmt, comment);
	set_val_cmt_(cfg);
	
	cfg->track_map_key = 52;
	cfg->cur_sel = cfg->track_map_key;
	get_val_cmt_(cfg);
	cfg->track_map_cmt = (char *) malloc(sizeof(char) * MAX_LEN);
	cfg->track_map = (int) value;
	strcpy(cfg->track_map_cmt, comment);
	set_val_cmt_(cfg);

	cfg->env_map_key = 64;
	cfg->cur_sel = cfg->env_map_key;
	get_val_cmt_(cfg);
	cfg->env_map_cmt = (char *) malloc(sizeof(char) * MAX_LEN);
	cfg->env_map = (int) value;
	strcpy(cfg->env_map_cmt, comment);
	set_val_cmt_(cfg);

	cfg->tex_qlty_key = 23;
	cfg->cur_sel = cfg->tex_qlty_key;
	get_val_cmt_(cfg);
	cfg->tex_qlty_cmt = (char *) malloc(sizeof(char) * MAX_LEN);
	cfg->tex_qlty = (int) value;
	strcpy(cfg->tex_qlty_cmt, comment);
	set_val_cmt_(cfg);

	cfg->an_fil_qlty_key = 100;
	cfg->cur_sel = cfg->an_fil_qlty_key;
	get_val_cmt_(cfg);
	cfg->an_fil_qlty_cmt = (char *) malloc(sizeof(char) * MAX_LEN);
	cfg->an_fil_qlty = (int) value;
	strcpy(cfg->an_fil_qlty_cmt, comment);
	set_val_cmt_(cfg);

	cfg->shdw_type_key = 108;
	cfg->cur_sel = cfg->shdw_type_key;
	get_val_cmt_(cfg);
	cfg->shdw_type_cmt = (char *) malloc(sizeof(char) * MAX_LEN);
	cfg->shdw_type = atoi(value);

	strcpy(cfg->shdw_type_cmt, comment);
	set_val_cmt_(cfg);
	
	/*printf("Initialized values from file..\n");
	start_(cfg);*/
	MessageBox(NULL, "Initialized values from file..", "Notice", MB_ICONINFORMATION | MB_OK);
}

void free_vals(CFG *cfg) {
	free(cfg->tex_fil_qlty_cmt);
	free(cfg->track_map_cmt);
	free(cfg->env_map_cmt);
	free(cfg->tex_qlty_cmt);
	free(cfg->an_fil_qlty_cmt);
	free(cfg->shdw_type_cmt);
}

void read_file(CFG *cfg) {
	int num;
	FILE *handler;
	printf("Now reading contents of file..\n");

	cfg->tot_lines = 0;
	//free(cfg->all_lines);
	//free(cfg->cur_line);
	/*memset(cfg->all_lines, 0, sizeof(char) * MAX_LINE * MAX_LINES);
	memset(cfg->cur_line, 0, sizeof(char) * MAX_LINE);*/
	cfg->all_lines = (char **) malloc(sizeof(char) * MAX_LINE * MAX_LINES);
	cfg->cur_line = (char *) malloc(sizeof(char) * MAX_LINE);

	while(fgets(cfg->cur_line, MAX_LINE, cfg->fp)) {
		strip_newline(cfg->cur_line, MAX_LINE);
		if(strlen(cfg->cur_line) > 0) {
			cfg->all_lines[cfg->tot_lines] = (char *) malloc(sizeof(char) * MAX_LINE);
			strcpy(cfg->all_lines[cfg->tot_lines], cfg->cur_line);
			free(cfg->cur_line);
			cfg->cur_line = (char *) malloc(sizeof(char *) * MAX_LINE);
			++cfg->tot_lines;
		}
	}
	fclose(cfg->fp);

	printf("Now creating backup file..\n");
	handler = fopen("f1graphics.cfg.backup", "w");
	if(handler) {
		for(num=0; num<cfg->tot_lines; num++) {
			fprintf(handler, "%s\n", cfg->all_lines[num]);
		}
		fclose(handler);
		//printf("Created backup file successfully..\n");
		MessageBox(NULL, "Created backup file successfully..", "Notice", MB_ICONINFORMATION | MB_OK);
	}

	init_vals(cfg);
}	

/*int main(int argc, char *argv[], char *env[]) {
	CFG cfg;
	printf("\n|+++++++++++++++++++++++++++++|");
	printf("\n| Welcome to GP4 configurator |");
	printf("\n|+++++++++++++++++++++++++++++|\n");

	printf("Specify the path for the configuration file: ");
	memset(cfg.file_name, 0, 256);
	fgets(cfg.file_name, 256, stdin);
	strip_newline(cfg.file_name, 256);

	if(strlen(cfg.file_name) > 0) {
		cfg.fp = fopen(cfg.file_name, "r");
		if(cfg.fp) {
			printf("Loading from location: %s\n", cfg.file_name);
			read_file(&cfg);
		} else {
			printf("Error reading file!\n");
		}
	} else {
		strcpy(cfg.file_name, "C:\\Program Files\\Infogrames\\Grand Prix 4\\f1graphics.cfg");
		cfg.fp = fopen(cfg.file_name, "r");
		if(!cfg.fp) {
			strcpy(cfg.file_name, "C:\\Program Files (x86)\\Infogrames\\Grand Prix 4\\f1graphics.cfg");
			cfg.fp = fopen(cfg.file_name, "r");
			if(!cfg.fp) {
				printf("Error reading file!\n");
				return -1;
			}
		}
		printf("Loading from default location: %s\n", cfg.file_name);
		read_file(&cfg);
		free_vals(&cfg);
		free(cfg.cur_line);
		free(cfg.all_lines);
		cfg.tot_lines = 0;
	}

	if(sizeof(buff) > 0) free(buff);
	if(sizeof(value) > 0) free(value);
	if(sizeof(comment) > 0) free(comment);
	
	return 0;
}*/

/*void strip_newtab(char *str, int len) {
	int i = 0;
	for(i=0; i<len; i++) {
		if(str[i] == '\t') {
			str[i] = '\0';
			break;
		}
	}
}

void strip_newline(char *str, int len) {
	int i;
	for(i=0; i<len; i++) {
		if(str[i] == '\n') {
			str[i] = '\0';
			break;
		}
	}
}*/

void SetWndProperties(WNDCLASSEX *wc, HINSTANCE *hInstance, const char *g_szClassName);

BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	switch(Msg) {
		case WM_INITDIALOG:
			return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDOK:
					EndDialog(hwnd, IDOK);
					break;
				case IDCANCEL:
					EndDialog(hwnd, IDCANCEL);
					break;
			}
			break;
		default:
			return FALSE;
			break;
	}
	return TRUE;
}

BOOL CALLBACK ToolDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDC_PRESS:
					break;
				case IDC_OTHER:
					break;
			}
			break;
		default:
			return FALSE;
	}
	return TRUE;
}

void SetFont(HWND *hwnd, HFONT *hDefault) {
	SendMessage(*hwnd, WM_SETFONT, (WPARAM) *hDefault, MAKELPARAM(0, FALSE));
}

HWND CreateMainWnd(HINSTANCE *hInstance, const char *className, char *wndName, int w, int h) {
	HWND hwnd = CreateWindowEx(
						WS_EX_CLIENTEDGE,
						className,
						wndName,
						WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,
						CW_USEDEFAULT, CW_USEDEFAULT, w, h,
						NULL, NULL, *hInstance, NULL);
	if(hwnd == NULL) {
		MessageBox(NULL, "Failed to create main window!", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	return hwnd;
}

HWND CreateWndLabel(HWND mainWnd, char *wndName, int l, int t, int w, int h, int id) {
	HWND hLabel = CreateWindowEx(
							0,
							"STATIC",
							wndName,
							WS_CHILD | WS_VISIBLE | SS_LEFT,
							l, t, w, h,
							mainWnd, (HMENU) id, GetModuleHandle(NULL), NULL);
	if(hLabel == NULL) {
		MessageBox(mainWnd, "Failed to create label!", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	return hLabel;
}

HWND CreateWndEdit(HWND mainWnd, char *wndName, int l, int t, int w, int h, int id) {
	HWND hEdit = CreateWindowEx(
							WS_EX_CLIENTEDGE,
							"EDIT",
							wndName,
							WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
							l, t, w, h,
							mainWnd, (HMENU) id, GetModuleHandle(NULL), NULL);
	if(hEdit == NULL) {
		MessageBox(mainWnd, "Failed to create edit box!", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	return hEdit;
}

HWND CreateWndCombo(HWND mainWnd, char *wndName, int l, int t, int w, int h, int id) {
	HWND hCombo = CreateWindowEx(
							WS_EX_CLIENTEDGE,
							"COMBOBOX",
							wndName,
							WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
							l, t, w, h,
							mainWnd, (HMENU) id, GetModuleHandle(NULL), NULL);
	if(hCombo == NULL) {
		MessageBox(mainWnd, "Failed to create combo box!", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	return hCombo;
}

HWND CreateWndList(HWND mainWnd, char *wndName, int l, int t, int w, int h, int id) {
	HWND hList = CreateWindowEx(
							WS_EX_CLIENTEDGE,
							"LISTBOX",
							wndName,
							WS_CHILD | WS_VISIBLE,
							l, t, w, h,
							mainWnd, (HMENU) id, GetModuleHandle(NULL), NULL);
	if(hList == NULL) {
		MessageBox(mainWnd, "Failed to create list box!", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	return hList;
}

HWND CreateWndButton(HWND mainWnd, char *wndName, int l, int t, int w, int h, int id) {
	HWND hButton = CreateWindowEx(
							WS_EX_CLIENTEDGE,
							"BUTTON",
							wndName,
							WS_CHILD | WS_VISIBLE,
							l, t, w, h,
							mainWnd, (HMENU) id, GetModuleHandle(NULL), NULL);
	if(hButton == NULL) {
		MessageBox(mainWnd, "Failed to create button!", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	return hButton;
}

BOOL LoadTextFileToEdit(HWND hEdit, LPCTSTR g_szFileName) {
	HANDLE hFile;
	BOOL bSuccess = FALSE;

	hFile = CreateFile(g_szFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
					OPEN_EXISTING, 0, NULL);

	if(hFile != INVALID_HANDLE_VALUE) {
		DWORD dwFileSize;
		dwFileSize = GetFileSize(hFile, NULL);
		if(dwFileSize != 0xFFFFFFFF) {
			LPSTR lpszFileText;
			lpszFileText = GlobalAlloc(GPTR, dwFileSize + 1);
			if(lpszFileText != NULL) {
				DWORD dwRead;
				if(ReadFile(hFile, lpszFileText, dwFileSize, &dwRead, NULL)) {
					lpszFileText[dwFileSize] = 0;
					if(SetWindowText(hEdit, lpszFileText)) {
						bSuccess = TRUE;
					}
				}
				GlobalFree(lpszFileText);
			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}

BOOL file_exists(char *filename) {
	FILE *fp;
	fp = fopen(filename, "r");
	if(fp) return 1;
	else return 0;
}

HWND hLabel, hTexLbl, hTexture, hBMLbl, hBumpMap, hShdLbl, hShadows, hHHLbl, hHeatHaze, hDtlLbl, hDetail, hFrmLbl, hFrames, hEdit;
HWND hTexFilQ, hTrkMp, hEnvMp, hTexQ, hMxAnFil, hShdwT;
HWND hTexFilQLbl, hTrkMpLbl, hEnvMpLbl, hTexQLbl, hMxAnFilLbl, hShdwTLbl;

HWND hUButton, hDButton, hLButton;
CFG cfg;

void PreloadSettings(CFG *cfg) {
	SendMessage(hTexFilQ, CB_SETCURSEL, cfg->tex_fil_qlty, 0);
	SendMessage(hTrkMp, CB_SETCURSEL, cfg->track_map, 0);
	SendMessage(hEnvMp, CB_SETCURSEL, cfg->env_map, 0);
	SendMessage(hTexQ, CB_SETCURSEL, cfg->tex_qlty, 0);
	SendMessage(hMxAnFil, CB_SETCURSEL, cfg->an_fil_qlty, 0);
	SendMessage(hShdwT, CB_SETCURSEL, cfg->shdw_type, 0);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
		case WM_CREATE:
			{
				hDefault = GetStockObject(DEFAULT_GUI_FONT);
				g_hbrBackground = (HBRUSH) CreateSolidBrush(RGB(255, 255, 255));
				strip_tab("Hello world\n", 20);
			}
			{
				hLabel = CreateWndLabel(hwnd, "Grand Prix 4 graphics", 10, 10, 200, 20, IDC_LABEL);
				if(!hLabel) return 0;
				SetFont(&hLabel, &hDefault);
			}
			{
				hTexFilQLbl = CreateWndLabel(hwnd, "Texture Filter Quality", 10, 40, 200, 20, IDC_TEXFILQLBL);
				if(!hTexFilQLbl) return 0;
				SetFont(&hTexFilQLbl, &hDefault);
			}
			{
				hTexFilQ = CreateWndCombo(hwnd, "", 10, 60, 200, 500, IDC_TEXFILQ);
				if(!hTexFilQ) return 0;
				SetFont(&hTexFilQ, &hDefault);
				SendMessage(hTexFilQ, CB_ADDSTRING, 0, (LPARAM) "None");
				SendMessage(hTexFilQ, CB_ADDSTRING, 1, (LPARAM) "Point");
				SendMessage(hTexFilQ, CB_ADDSTRING, 2, (LPARAM) "Bilinear");
				SendMessage(hTexFilQ, CB_ADDSTRING, 3, (LPARAM) "Anisotropic");
				SendMessage(hTexFilQ, CB_ADDSTRING, 4, (LPARAM) "Flat Cubic");
				SendMessage(hTexFilQ, CB_ADDSTRING, 5, (LPARAM) "Gaussian Cubic");
				SendMessage(hTexFilQ, CB_SETCURSEL, 0, 0);
			}
			{
				hTrkMpLbl = CreateWndLabel(hwnd, "Track Map", 240, 40, 200, 20, IDC_TRKMPLBL);
				if(!hTrkMpLbl) return 0;
				SetFont(&hTrkMpLbl, &hDefault);
			}
			{
				hTrkMp = CreateWndCombo(hwnd, "", 240, 60, 200, 500, IDC_TRKMP);
				if(!hTrkMp) return 0;
				SetFont(&hTrkMp, &hDefault);
				SendMessage(hTrkMp, CB_ADDSTRING, 0, (LPARAM) "Off");
				SendMessage(hTrkMp, CB_ADDSTRING, 1, (LPARAM) "Arcade Only");
				SendMessage(hTrkMp, CB_ADDSTRING, 2, (LPARAM) "All non trackside");
				SendMessage(hTrkMp, CB_SETCURSEL, 0, 0);
			}
			{
				hEnvMpLbl = CreateWndLabel(hwnd, "Environment Map", 470, 40, 200, 20, IDC_ENVMPLBL);
				if(!hEnvMpLbl) return 0;
				SetFont(&hEnvMpLbl, &hDefault);
			}
			{
				hEnvMp = CreateWndCombo(hwnd, "", 470, 60, 200, 500, IDC_ENVMP);
				if(!hEnvMp) return 0;
				SetFont(&hEnvMp, &hDefault);
				SendMessage(hEnvMp, CB_ADDSTRING, 0, (LPARAM) "Default");
				SendMessage(hEnvMp, CB_ADDSTRING, 1, (LPARAM) "Extra");
				SendMessage(hEnvMp, CB_SETCURSEL, 0, 0);
			}
			{
				hTexQLbl = CreateWndLabel(hwnd, "Texture Quality", 10, 120, 200, 20, IDC_TEXQLBL);
				if(!hTexQLbl) return 0;
				SetFont(&hTexQLbl, &hDefault);
			}
			{
				hTexQ = CreateWndCombo(hwnd, "", 10, 140, 200, 500, IDC_TEXQ);
				if(!hTexQ) return 0;
				SetFont(&hTexQ, &hDefault);
				SendMessage(hTexQ, CB_ADDSTRING, 0, (LPARAM) "MAX");
				SendMessage(hTexQ, CB_ADDSTRING, 2, (LPARAM) "HALF");
				SendMessage(hTexQ, CB_ADDSTRING, 4, (LPARAM) "QTR");
				SendMessage(hTexQ, CB_ADDSTRING, 8, (LPARAM) "EIGTH");
				SendMessage(hTexQ, CB_SETCURSEL, 0, 0);
			}
			{
				hMxAnFilLbl = CreateWndLabel(hwnd, "Max Anisotropic Filter (1-7)", 240, 120, 200, 20, IDC_MXANFILLBL);
				if(!hMxAnFilLbl) return 0;
				SetFont(&hMxAnFilLbl, &hDefault);
			}
			{
				hMxAnFil = CreateWndCombo(hwnd, "", 240, 140, 200, 500, IDC_MXANFIL);
				if(!hMxAnFil) return 0;
				SetFont(&hMxAnFil, &hDefault);
				SendMessage(hMxAnFil, CB_ADDSTRING, 1, (LPARAM) "Off");
				SendMessage(hMxAnFil, CB_ADDSTRING, 2, (LPARAM) "2");
				SendMessage(hMxAnFil, CB_ADDSTRING, 3, (LPARAM) "3");
				SendMessage(hMxAnFil, CB_ADDSTRING, 4, (LPARAM) "4");
				SendMessage(hMxAnFil, CB_ADDSTRING, 5, (LPARAM) "5");
				SendMessage(hMxAnFil, CB_ADDSTRING, 6, (LPARAM) "6");
				SendMessage(hMxAnFil, CB_ADDSTRING, 7, (LPARAM) "7");
				SendMessage(hMxAnFil, CB_SETCURSEL, 0, 0);
			}
			{
				hShdwTLbl = CreateWndLabel(hwnd, "Shadow Type", 470, 120, 200, 20, IDC_SHDWTLBL);
				if(!hShdwTLbl) return 0;
				SetFont(&hShdwTLbl, &hDefault);
			}
			{
				hShdwT = CreateWndCombo(hwnd, "", 470, 140, 200, 500, IDC_SHDWT);
				if(!hShdwT) return 0;
				SetFont(&hShdwT, &hDefault);
				SendMessage(hShdwT, CB_ADDSTRING, 0, (LPARAM) "Off");
				SendMessage(hShdwT, CB_ADDSTRING, 1, (LPARAM) "Static");
				SendMessage(hShdwT, CB_ADDSTRING, 2, (LPARAM) "Composite");
				SendMessage(hShdwT, CB_ADDSTRING, 3, (LPARAM) "Projected");
				SendMessage(hShdwT, CB_SETCURSEL, 0, 0);
			}
			{
				hUButton = CreateWndButton(hwnd, "Update", 10, 190, 60, 25, IDC_UPDBTN);
				SetFont(&hUButton, &hDefault);
			}
			{
				hDButton = CreateWndButton(hwnd, "Defaults", 80, 190, 60, 25, IDC_DEFBTN);
				SetFont(&hDButton, &hDefault);
			}
			{
				hLButton = CreateWndButton(hwnd, "Load File", 150, 190, 60, 25, IDC_LBTN);
				SetFont(&hLButton, &hDefault);
			}
			{
				hEdit = CreateWndEdit(hwnd, "", 0, 250, 100, 100, IDC_FEDIT);
				SetFont(&hEdit, &hDefault);
			}
			break;
		case WM_SIZE:
			{
				HWND hEdit;
				RECT rcEdit;

				RECT rcClient;
				int iEditHeight;

				hEdit = GetDlgItem(hwnd, IDC_FEDIT);
				GetWindowRect(hEdit, &rcEdit);

				GetClientRect(hwnd, &rcClient);

				iEditHeight = rcClient.bottom - 250;
				SetWindowPos(hEdit, NULL, 0, 250, rcClient.right, iEditHeight, SWP_NOZORDER);
			}
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case ID_HELP_ABOUT:
					{
						int ret = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT), hwnd, AboutDlgProc);
						if(ret == IDOK) {
							// TODO CODE
						} else if(ret == IDCANCEL) {
							// TODO CODE
						} else if(ret == -1) {
							MessageBox(hwnd, "Failed to create dialog box!", "Error", MB_ICONEXCLAMATION | MB_OK);
							return 0;
						}
					}
					break;
				case IDC_TEXFILQ:
					{
						switch(HIWORD(wParam)) {
							case CBN_SELCHANGE:
								//cfg.cur_sel = cfg.tex_fil_qlty_key;
								//cfg.tex_fil_qlty = SendMessage(hTexFilQ, CB_GETCURSEL, 0, 0);
								// TODO CODE
								//sprintf(text, "%d", cfg.tex_fil_qlty);
								//MessageBox(hwnd, text, "Selection", MB_OK);
								texfilq = SendMessage(hTexFilQ, CB_GETCURSEL, 0, 0);
								break;
						}
					}
					break;
				case IDC_TRKMP:
					{
						switch(HIWORD(wParam)) {
							case CBN_SELCHANGE:
								trkmp = SendMessage(hTrkMp, CB_GETCURSEL, 0, 0);
								break;
						}
					}
					break;
				case IDC_ENVMP:
					{
						switch(HIWORD(wParam)) {
							case CBN_SELCHANGE:
								envmp = SendMessage(hEnvMp, CB_GETCURSEL, 0, 0);
								break;
						}
					}
					break;
				case IDC_TEXQ:
					{
						switch(HIWORD(wParam)) {
							case CBN_SELCHANGE:
								texq = SendMessage(hTexQ, CB_GETCURSEL, 0, 0);
								break;
						}
					}
				case IDC_MXANFIL:
					{
						switch(HIWORD(wParam)) {
							case CBN_SELCHANGE:
								mxanfil = SendMessage(hMxAnFil, CB_GETCURSEL, 0, 0);
								break;
						}
					}
					break;
				case IDC_SHDWT:
					{
						switch(HIWORD(wParam)) {
							case CBN_SELCHANGE:
								shdwt = SendMessage(hShdwT, CB_GETCURSEL, 0, 0);
								break;
						}
					}
					break;
				case IDC_LBTN:
					{
						ZeroMemory(&ofn, sizeof(ofn));

						ofn.lStructSize = sizeof(ofn);
						ofn.hwndOwner = hwnd;
						//ofn.lpstrFilter = "Text files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
						ofn.lpstrFilter = "GP4 configuration files (*.cfg)\0*.cfg\0";
						ofn.lpstrFile = szFileName;
						ofn.nMaxFile = MAX_PATHE;
						ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
						ofn.lpstrDefExt = "cfg";

						if(GetOpenFileName(&ofn)) {
							if(!LoadTextFileToEdit(hEdit, szFileName)) {
								MessageBox(hwnd, "Failed to load text file!", "Error", MB_ICONERROR | MB_OK);
								return 0;
							}
							if(file_exists(szFileName)) {
								memset(cfg.file_name, 0, 256);
								strcpy(cfg.file_name, szFileName);
								//fp = fopen(szFileName, "r");
								cfg.fp = fopen(cfg.file_name, "r");
								if(cfg.fp) {
									//printf("Loading from location: %s\n", cfg.file_name);
									read_file(&cfg);
									PreloadSettings(&cfg);
								} else {
									//printf("Error reading file!\n");
									MessageBox(hwnd, "Cannot read file!", szFileName, MB_ICONERROR | MB_OK);
									return -1;
								}
								/*if(fp) {
									line = (char *) malloc(sizeof(char) * STRLEN);
									lines = (char **) malloc(sizeof(char) * MAX_LEN);
									i = 0;
									j = 0;
									while(fgets(line, 100, fp)) {
										strip_newline(line, 100);
										lines[i] = (char *) malloc(STRLEN);
										strcpy(lines[i], line);
										++i; ++j;
										free(line);
										line = (char *) malloc(STRLEN);
									}
									fclose(fp);
								} else {
									MessageBox(hwnd, "Cannot read file!", szFileName, MB_ICONERROR | MB_OK);
								}*/
							} else {
								MessageBox(hwnd, "File not found!", szFileName, MB_ICONERROR | MB_OK);
							}
							SetWindowText(hwnd, szFileName);
						}
					}
					break;
				case IDC_UPDBTN:
					//MessageBox(hwnd, "A new configuration file is created in C:\\GP4Configurator", "Configuration file created", MB_ICONINFORMATION | MB_OK);
					{
						/*char buf[5];
						BOOL bSuccess;
						GetDlgItemText(hwnd, IDC_TEXTURES, buf, 5);
						MessageBox(hwnd, buf, "Notice", MB_OK);*/
						
						/*ZeroMemory(&ofn, sizeof(ofn));

						ofn.lStructSize = sizeof(ofn);
						ofn.hwndOwner = hwnd;
						ofn.lpstrFile = szFileName;
						//ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
						ofn.lpstrFilter = "GP4 configuration files (*.cfg)\0*.cfg\0";
						ofn.nMaxFile = MAX_PATHE;
						ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
						ofn.lpstrDefExt = "cfg";

						if(GetSaveFileName(&ofn)) {
							fw = fopen(szFileName, "w");
							if(fw) {
								//fprintf(fw, "i is %d and j is %d\n", i, j);
								for(i=0; i<j; i++) {
									fprintf(fw, "%s\n", lines[i]);
								}
								fclose(fw);
							} else {
								MessageBox(hwnd, "Cannot write to file!", szFileName, MB_ICONERROR | MB_OK);
							}
						}*/
						cfg.tex_fil_qlty = texfilq;
						cfg.track_map = trkmp;
						cfg.env_map = envmp;
						cfg.tex_qlty = texq;
						cfg.an_fil_qlty = mxanfil;
						cfg.shdw_type = shdwt;
						MessageBox(hwnd, "New values updated to buffer..", "Notice", MB_ICONINFORMATION | MB_OK);
					}
					break;
				case ID_FILE_EXIT:
					PostMessage(hwnd, WM_QUIT, 0, 0);
					break;
				case IDT_SHOW:
					ShowWindow(g_hToolBar, SW_SHOW);
					break;
				case IDT_HIDE:
					ShowWindow(g_hToolBar, SW_HIDE);
					break;
			}
			break;
		case WM_CTLCOLORDLG:
			return (LONG) g_hbrBackground;
			break;
		case WM_CTLCOLORSTATIC:
			{
				hdcStatic = (HDC) wParam;
				SetTextColor(hdcStatic, RGB(0, 0, 0));
				SetBkMode(hdcStatic, TRANSPARENT);
				return (LONG) g_hbrBackground;
			}
			break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			DestroyWindow(g_hToolBar);
			if(fp) {
				free(line);
				free(lines);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
			break;
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	HWND hwnd = NULL;
	MSG Msg;

	SetWndProperties(&wc, &hInstance, g_szClassName);
	
	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Failed to register window class!", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateMainWnd(&hInstance, g_szClassName, "GP4 Graphics Configuration", 700, 650);

	if(hwnd == NULL) {
		//MessageBox(NULL, "Failed to create window!", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while(GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}

void SetWndProperties(WNDCLASSEX *wc, HINSTANCE *hInstance, const char *g_szClassName) {
	wc->cbSize = sizeof(WNDCLASSEX);
	wc->style = 0;
	wc->lpfnWndProc = WndProc;
	wc->cbClsExtra = 0;
	wc->cbWndExtra = 0;
	wc->hInstance = *hInstance;
	wc->hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc->hCursor = LoadCursor(NULL, IDC_ARROW);
	wc->hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wc->lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);
	wc->lpszClassName = g_szClassName;
	wc->hIconSm = LoadIcon(NULL, IDI_APPLICATION);
}

