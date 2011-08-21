/*#pragma once

namespace WorldOfWarcraftEditor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  menu;
	private: System::Windows::Forms::ToolStripMenuItem^  souborMainMenu;
	protected: 

	private: System::Windows::Forms::ToolStripMenuItem^  openMainMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  continentMainMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  dungeonMainMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  raidsMainMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  battlegroundMainMenu;





	private: System::Windows::Forms::ToolStripMenuItem^  saveAllMainMenu;

	private: System::Windows::Forms::ToolStripMenuItem^  saveCurrentMainMenu;

	private: System::Windows::Forms::ToolStripMenuItem^  reloadMainMenu;

	private: System::Windows::Forms::ToolStripMenuItem^  exitMainMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  viewMainMenu;













	private: System::Windows::Forms::ToolStripMenuItem^  toolbarUpMainMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  currentTextureMainMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  texturePaletteMainMenu;




	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  helpMainMenu;


	private: System::Windows::Forms::ToolStripMenuItem^  aboutWorldOfWarcraftEditorMainMenu;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator6;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator7;
	private: System::Windows::Forms::ToolStripMenuItem^  m2ObjectsMainMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  wMOObjectsMainMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  terrainMainMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  waterMainMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  linesMainMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  detailInfosMainMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  mapContourInfosMainMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  fogMainMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  hotelinesAlwaysOnMainMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  toolsMainMenu;














	private: System::Windows::Forms::ToolStripMenuItem^  importFromTXTMainMenu;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator9;
	private: System::Windows::Forms::ToolStripMenuItem^  cubeMainMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  discMainMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  sphereMainMenu;



	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator8;
private: System::Windows::Forms::ToolStripMenuItem^  setAreaIDMainMenu;
private: System::Windows::Forms::ToolStripMenuItem^  clearHeightMapMainMenu;
private: System::Windows::Forms::ToolStripMenuItem^  moveToPositionMainMenu;
private: System::Windows::Forms::ToolStripMenuItem^  clearTextureMainMenu;




	private: System::Windows::Forms::ToolStrip^  tools1;
private: System::Windows::Forms::ToolStripButton^  openTools1;

	private: System::Windows::Forms::ToolStrip^  tools2;
private: System::Windows::Forms::ToolStripButton^  MoveTools2;

private: System::Windows::Forms::ToolStripButton^  TerrainTools2;

private: System::Windows::Forms::ToolStripButton^  PaintingTools2;

private: System::Windows::Forms::ToolStripButton^  EraserTools2;

private: System::Windows::Forms::ToolStripButton^  DeletingAddingTextureTools2;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator10;
	private: System::Windows::Forms::ToolStripMenuItem^  toolbarLeftMainMenu;

	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator11;
private: System::Windows::Forms::ToolStripButton^  saveTools1;
private: System::Windows::Forms::ToolStripButton^  reloadTools1;


	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator12;
private: System::Windows::Forms::ToolStripButton^  undoTools1;
private: System::Windows::Forms::ToolStripButton^  redoTools1;


private: System::Windows::Forms::ToolStripMenuItem^  editMainMenu;

private: System::Windows::Forms::ToolStripMenuItem^  undoMainMenu;
private: System::Windows::Forms::ToolStripMenuItem^  redoMainMenu;
private: System::Windows::Forms::ToolStripMenuItem^  copyMainMenu;
private: System::Windows::Forms::ToolStripMenuItem^  pasteMainMenu;
private: System::Windows::Forms::ToolStripMenuItem^  deleteMainMenu;





	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
private: System::Windows::Forms::ToolStripMenuItem^  resetMainMenu;
private: System::Windows::Forms::ToolStripMenuItem^  autoSelectMainMenu;


	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator4;
private: System::Windows::Forms::ToolStripMenuItem^  copyRandomSizeMainMenu;
private: System::Windows::Forms::ToolStripMenuItem^  copyRandomTileMainMenu;
private: System::Windows::Forms::ToolStripMenuItem^  copyRandomRotationMainMenu;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator5;
private: System::Windows::Forms::ToolStripMenuItem^  keybindingsMainMenu;
private: System::Windows::Forms::ToolStripButton^  AreaIDTools2;








	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->menu = (gcnew System::Windows::Forms::MenuStrip());
			this->souborMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->continentMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->dungeonMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->raidsMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->battlegroundMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->saveAllMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveCurrentMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator6 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->reloadMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator7 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->exitMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->undoMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->redoMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->copyMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pasteMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->deleteMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->resetMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->autoSelectMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator4 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->copyRandomSizeMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->copyRandomTileMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->copyRandomRotationMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator5 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->keybindingsMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->viewMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolbarUpMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolbarLeftMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator11 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->currentTextureMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->texturePaletteMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->m2ObjectsMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->wMOObjectsMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->terrainMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->waterMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->linesMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->detailInfosMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mapContourInfosMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->fogMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->hotelinesAlwaysOnMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolsMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->importFromTXTMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator9 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->cubeMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->discMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->sphereMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator8 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->setAreaIDMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->clearHeightMapMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->moveToPositionMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->clearTextureMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutWorldOfWarcraftEditorMainMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tools1 = (gcnew System::Windows::Forms::ToolStrip());
			this->openTools1 = (gcnew System::Windows::Forms::ToolStripButton());
			this->saveTools1 = (gcnew System::Windows::Forms::ToolStripButton());
			this->reloadTools1 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator12 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->undoTools1 = (gcnew System::Windows::Forms::ToolStripButton());
			this->redoTools1 = (gcnew System::Windows::Forms::ToolStripButton());
			this->tools2 = (gcnew System::Windows::Forms::ToolStrip());
			this->MoveTools2 = (gcnew System::Windows::Forms::ToolStripButton());
			this->TerrainTools2 = (gcnew System::Windows::Forms::ToolStripButton());
			this->PaintingTools2 = (gcnew System::Windows::Forms::ToolStripButton());
			this->EraserTools2 = (gcnew System::Windows::Forms::ToolStripButton());
			this->DeletingAddingTextureTools2 = (gcnew System::Windows::Forms::ToolStripButton());
			this->AreaIDTools2 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator10 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->menu->SuspendLayout();
			this->tools1->SuspendLayout();
			this->tools2->SuspendLayout();
			this->SuspendLayout();
			// 
			// menu
			// 
			this->menu->BackColor = System::Drawing::Color::Transparent;
			this->menu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->souborMainMenu, this->editMainMenu, 
				this->viewMainMenu, this->toolsMainMenu, this->helpMainMenu});
			resources->ApplyResources(this->menu, L"menu");
			this->menu->Name = L"menu";
			// 
			// souborMainMenu
			// 
			this->souborMainMenu->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(8) {this->openMainMenu, 
				this->toolStripSeparator2, this->saveAllMainMenu, this->saveCurrentMainMenu, this->toolStripSeparator6, this->reloadMainMenu, 
				this->toolStripSeparator7, this->exitMainMenu});
			this->souborMainMenu->Name = L"souborMainMenu";
			resources->ApplyResources(this->souborMainMenu, L"souborMainMenu");
			// 
			// openMainMenu
			// 
			this->openMainMenu->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->continentMainMenu, 
				this->dungeonMainMenu, this->raidsMainMenu, this->battlegroundMainMenu});
			resources->ApplyResources(this->openMainMenu, L"openMainMenu");
			this->openMainMenu->Name = L"openMainMenu";
			// 
			// continentMainMenu
			// 
			this->continentMainMenu->Name = L"continentMainMenu";
			resources->ApplyResources(this->continentMainMenu, L"continentMainMenu");
			// 
			// dungeonMainMenu
			// 
			this->dungeonMainMenu->Name = L"dungeonMainMenu";
			resources->ApplyResources(this->dungeonMainMenu, L"dungeonMainMenu");
			// 
			// raidsMainMenu
			// 
			this->raidsMainMenu->Name = L"raidsMainMenu";
			resources->ApplyResources(this->raidsMainMenu, L"raidsMainMenu");
			// 
			// battlegroundMainMenu
			// 
			this->battlegroundMainMenu->Name = L"battlegroundMainMenu";
			resources->ApplyResources(this->battlegroundMainMenu, L"battlegroundMainMenu");
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			resources->ApplyResources(this->toolStripSeparator2, L"toolStripSeparator2");
			// 
			// saveAllMainMenu
			// 
			resources->ApplyResources(this->saveAllMainMenu, L"saveAllMainMenu");
			this->saveAllMainMenu->Name = L"saveAllMainMenu";
			// 
			// saveCurrentMainMenu
			// 
			resources->ApplyResources(this->saveCurrentMainMenu, L"saveCurrentMainMenu");
			this->saveCurrentMainMenu->Name = L"saveCurrentMainMenu";
			// 
			// toolStripSeparator6
			// 
			this->toolStripSeparator6->Name = L"toolStripSeparator6";
			resources->ApplyResources(this->toolStripSeparator6, L"toolStripSeparator6");
			// 
			// reloadMainMenu
			// 
			resources->ApplyResources(this->reloadMainMenu, L"reloadMainMenu");
			this->reloadMainMenu->Name = L"reloadMainMenu";
			// 
			// toolStripSeparator7
			// 
			this->toolStripSeparator7->Name = L"toolStripSeparator7";
			resources->ApplyResources(this->toolStripSeparator7, L"toolStripSeparator7");
			// 
			// exitMainMenu
			// 
			resources->ApplyResources(this->exitMainMenu, L"exitMainMenu");
			this->exitMainMenu->Name = L"exitMainMenu";
			// 
			// editMainMenu
			// 
			this->editMainMenu->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(14) {this->undoMainMenu, 
				this->redoMainMenu, this->copyMainMenu, this->pasteMainMenu, this->deleteMainMenu, this->toolStripSeparator3, this->resetMainMenu, 
				this->autoSelectMainMenu, this->toolStripSeparator4, this->copyRandomSizeMainMenu, this->copyRandomTileMainMenu, this->copyRandomRotationMainMenu, 
				this->toolStripSeparator5, this->keybindingsMainMenu});
			this->editMainMenu->Name = L"editMainMenu";
			resources->ApplyResources(this->editMainMenu, L"editMainMenu");
			// 
			// undoMainMenu
			// 
			resources->ApplyResources(this->undoMainMenu, L"undoMainMenu");
			this->undoMainMenu->Name = L"undoMainMenu";
			// 
			// redoMainMenu
			// 
			resources->ApplyResources(this->redoMainMenu, L"redoMainMenu");
			this->redoMainMenu->Name = L"redoMainMenu";
			// 
			// copyMainMenu
			// 
			resources->ApplyResources(this->copyMainMenu, L"copyMainMenu");
			this->copyMainMenu->Name = L"copyMainMenu";
			// 
			// pasteMainMenu
			// 
			resources->ApplyResources(this->pasteMainMenu, L"pasteMainMenu");
			this->pasteMainMenu->Name = L"pasteMainMenu";
			// 
			// deleteMainMenu
			// 
			resources->ApplyResources(this->deleteMainMenu, L"deleteMainMenu");
			this->deleteMainMenu->Name = L"deleteMainMenu";
			// 
			// toolStripSeparator3
			// 
			this->toolStripSeparator3->Name = L"toolStripSeparator3";
			resources->ApplyResources(this->toolStripSeparator3, L"toolStripSeparator3");
			// 
			// resetMainMenu
			// 
			resources->ApplyResources(this->resetMainMenu, L"resetMainMenu");
			this->resetMainMenu->Name = L"resetMainMenu";
			// 
			// autoSelectMainMenu
			// 
			this->autoSelectMainMenu->CheckOnClick = true;
			resources->ApplyResources(this->autoSelectMainMenu, L"autoSelectMainMenu");
			this->autoSelectMainMenu->Name = L"autoSelectMainMenu";
			// 
			// toolStripSeparator4
			// 
			this->toolStripSeparator4->Name = L"toolStripSeparator4";
			resources->ApplyResources(this->toolStripSeparator4, L"toolStripSeparator4");
			// 
			// copyRandomSizeMainMenu
			// 
			this->copyRandomSizeMainMenu->CheckOnClick = true;
			this->copyRandomSizeMainMenu->Name = L"copyRandomSizeMainMenu";
			resources->ApplyResources(this->copyRandomSizeMainMenu, L"copyRandomSizeMainMenu");
			// 
			// copyRandomTileMainMenu
			// 
			this->copyRandomTileMainMenu->CheckOnClick = true;
			this->copyRandomTileMainMenu->Name = L"copyRandomTileMainMenu";
			resources->ApplyResources(this->copyRandomTileMainMenu, L"copyRandomTileMainMenu");
			// 
			// copyRandomRotationMainMenu
			// 
			this->copyRandomRotationMainMenu->CheckOnClick = true;
			this->copyRandomRotationMainMenu->Name = L"copyRandomRotationMainMenu";
			resources->ApplyResources(this->copyRandomRotationMainMenu, L"copyRandomRotationMainMenu");
			// 
			// toolStripSeparator5
			// 
			this->toolStripSeparator5->Name = L"toolStripSeparator5";
			resources->ApplyResources(this->toolStripSeparator5, L"toolStripSeparator5");
			// 
			// keybindingsMainMenu
			// 
			resources->ApplyResources(this->keybindingsMainMenu, L"keybindingsMainMenu");
			this->keybindingsMainMenu->Name = L"keybindingsMainMenu";
			// 
			// viewMainMenu
			// 
			this->viewMainMenu->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(15) {this->toolbarUpMainMenu, 
				this->toolbarLeftMainMenu, this->toolStripSeparator11, this->currentTextureMainMenu, this->texturePaletteMainMenu, this->toolStripSeparator1, 
				this->m2ObjectsMainMenu, this->wMOObjectsMainMenu, this->terrainMainMenu, this->waterMainMenu, this->linesMainMenu, this->detailInfosMainMenu, 
				this->mapContourInfosMainMenu, this->fogMainMenu, this->hotelinesAlwaysOnMainMenu});
			this->viewMainMenu->Name = L"viewMainMenu";
			resources->ApplyResources(this->viewMainMenu, L"viewMainMenu");
			// 
			// toolbarUpMainMenu
			// 
			this->toolbarUpMainMenu->Checked = true;
			this->toolbarUpMainMenu->CheckOnClick = true;
			this->toolbarUpMainMenu->CheckState = System::Windows::Forms::CheckState::Checked;
			resources->ApplyResources(this->toolbarUpMainMenu, L"toolbarUpMainMenu");
			this->toolbarUpMainMenu->Name = L"toolbarUpMainMenu";
			// 
			// toolbarLeftMainMenu
			// 
			this->toolbarLeftMainMenu->Checked = true;
			this->toolbarLeftMainMenu->CheckOnClick = true;
			this->toolbarLeftMainMenu->CheckState = System::Windows::Forms::CheckState::Checked;
			resources->ApplyResources(this->toolbarLeftMainMenu, L"toolbarLeftMainMenu");
			this->toolbarLeftMainMenu->Name = L"toolbarLeftMainMenu";
			// 
			// toolStripSeparator11
			// 
			this->toolStripSeparator11->Name = L"toolStripSeparator11";
			resources->ApplyResources(this->toolStripSeparator11, L"toolStripSeparator11");
			// 
			// currentTextureMainMenu
			// 
			this->currentTextureMainMenu->CheckOnClick = true;
			this->currentTextureMainMenu->Name = L"currentTextureMainMenu";
			resources->ApplyResources(this->currentTextureMainMenu, L"currentTextureMainMenu");
			// 
			// texturePaletteMainMenu
			// 
			this->texturePaletteMainMenu->CheckOnClick = true;
			this->texturePaletteMainMenu->Name = L"texturePaletteMainMenu";
			resources->ApplyResources(this->texturePaletteMainMenu, L"texturePaletteMainMenu");
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			resources->ApplyResources(this->toolStripSeparator1, L"toolStripSeparator1");
			// 
			// m2ObjectsMainMenu
			// 
			this->m2ObjectsMainMenu->CheckOnClick = true;
			this->m2ObjectsMainMenu->Name = L"m2ObjectsMainMenu";
			resources->ApplyResources(this->m2ObjectsMainMenu, L"m2ObjectsMainMenu");
			// 
			// wMOObjectsMainMenu
			// 
			this->wMOObjectsMainMenu->CheckOnClick = true;
			this->wMOObjectsMainMenu->Name = L"wMOObjectsMainMenu";
			resources->ApplyResources(this->wMOObjectsMainMenu, L"wMOObjectsMainMenu");
			// 
			// terrainMainMenu
			// 
			this->terrainMainMenu->CheckOnClick = true;
			this->terrainMainMenu->Name = L"terrainMainMenu";
			resources->ApplyResources(this->terrainMainMenu, L"terrainMainMenu");
			// 
			// waterMainMenu
			// 
			this->waterMainMenu->CheckOnClick = true;
			this->waterMainMenu->Name = L"waterMainMenu";
			resources->ApplyResources(this->waterMainMenu, L"waterMainMenu");
			// 
			// linesMainMenu
			// 
			this->linesMainMenu->CheckOnClick = true;
			this->linesMainMenu->Name = L"linesMainMenu";
			resources->ApplyResources(this->linesMainMenu, L"linesMainMenu");
			// 
			// detailInfosMainMenu
			// 
			this->detailInfosMainMenu->CheckOnClick = true;
			this->detailInfosMainMenu->Name = L"detailInfosMainMenu";
			resources->ApplyResources(this->detailInfosMainMenu, L"detailInfosMainMenu");
			// 
			// mapContourInfosMainMenu
			// 
			this->mapContourInfosMainMenu->CheckOnClick = true;
			this->mapContourInfosMainMenu->Name = L"mapContourInfosMainMenu";
			resources->ApplyResources(this->mapContourInfosMainMenu, L"mapContourInfosMainMenu");
			// 
			// fogMainMenu
			// 
			this->fogMainMenu->CheckOnClick = true;
			this->fogMainMenu->Name = L"fogMainMenu";
			resources->ApplyResources(this->fogMainMenu, L"fogMainMenu");
			// 
			// hotelinesAlwaysOnMainMenu
			// 
			this->hotelinesAlwaysOnMainMenu->CheckOnClick = true;
			this->hotelinesAlwaysOnMainMenu->Name = L"hotelinesAlwaysOnMainMenu";
			resources->ApplyResources(this->hotelinesAlwaysOnMainMenu, L"hotelinesAlwaysOnMainMenu");
			// 
			// toolsMainMenu
			// 
			this->toolsMainMenu->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(10) {this->importFromTXTMainMenu, 
				this->toolStripSeparator9, this->cubeMainMenu, this->discMainMenu, this->sphereMainMenu, this->toolStripSeparator8, this->setAreaIDMainMenu, 
				this->clearHeightMapMainMenu, this->moveToPositionMainMenu, this->clearTextureMainMenu});
			this->toolsMainMenu->Name = L"toolsMainMenu";
			resources->ApplyResources(this->toolsMainMenu, L"toolsMainMenu");
			// 
			// importFromTXTMainMenu
			// 
			resources->ApplyResources(this->importFromTXTMainMenu, L"importFromTXTMainMenu");
			this->importFromTXTMainMenu->Name = L"importFromTXTMainMenu";
			// 
			// toolStripSeparator9
			// 
			this->toolStripSeparator9->Name = L"toolStripSeparator9";
			resources->ApplyResources(this->toolStripSeparator9, L"toolStripSeparator9");
			// 
			// cubeMainMenu
			// 
			this->cubeMainMenu->Name = L"cubeMainMenu";
			resources->ApplyResources(this->cubeMainMenu, L"cubeMainMenu");
			// 
			// discMainMenu
			// 
			resources->ApplyResources(this->discMainMenu, L"discMainMenu");
			this->discMainMenu->Name = L"discMainMenu";
			// 
			// sphereMainMenu
			// 
			resources->ApplyResources(this->sphereMainMenu, L"sphereMainMenu");
			this->sphereMainMenu->Name = L"sphereMainMenu";
			// 
			// toolStripSeparator8
			// 
			this->toolStripSeparator8->Name = L"toolStripSeparator8";
			resources->ApplyResources(this->toolStripSeparator8, L"toolStripSeparator8");
			// 
			// setAreaIDMainMenu
			// 
			this->setAreaIDMainMenu->Name = L"setAreaIDMainMenu";
			resources->ApplyResources(this->setAreaIDMainMenu, L"setAreaIDMainMenu");
			// 
			// clearHeightMapMainMenu
			// 
			this->clearHeightMapMainMenu->Name = L"clearHeightMapMainMenu";
			resources->ApplyResources(this->clearHeightMapMainMenu, L"clearHeightMapMainMenu");
			// 
			// moveToPositionMainMenu
			// 
			resources->ApplyResources(this->moveToPositionMainMenu, L"moveToPositionMainMenu");
			this->moveToPositionMainMenu->Name = L"moveToPositionMainMenu";
			// 
			// clearTextureMainMenu
			// 
			this->clearTextureMainMenu->Name = L"clearTextureMainMenu";
			resources->ApplyResources(this->clearTextureMainMenu, L"clearTextureMainMenu");
			// 
			// helpMainMenu
			// 
			this->helpMainMenu->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->aboutWorldOfWarcraftEditorMainMenu});
			this->helpMainMenu->Name = L"helpMainMenu";
			resources->ApplyResources(this->helpMainMenu, L"helpMainMenu");
			// 
			// aboutWorldOfWarcraftEditorMainMenu
			// 
			resources->ApplyResources(this->aboutWorldOfWarcraftEditorMainMenu, L"aboutWorldOfWarcraftEditorMainMenu");
			this->aboutWorldOfWarcraftEditorMainMenu->Name = L"aboutWorldOfWarcraftEditorMainMenu";
			// 
			// tools1
			// 
			this->tools1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->openTools1, this->saveTools1, 
				this->reloadTools1, this->toolStripSeparator12, this->undoTools1, this->redoTools1});
			resources->ApplyResources(this->tools1, L"tools1");
			this->tools1->Name = L"tools1";
			// 
			// openTools1
			// 
			this->openTools1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->openTools1, L"openTools1");
			this->openTools1->Name = L"openTools1";
			// 
			// saveTools1
			// 
			this->saveTools1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->saveTools1, L"saveTools1");
			this->saveTools1->Name = L"saveTools1";
			// 
			// reloadTools1
			// 
			this->reloadTools1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->reloadTools1, L"reloadTools1");
			this->reloadTools1->Name = L"reloadTools1";
			// 
			// toolStripSeparator12
			// 
			this->toolStripSeparator12->Name = L"toolStripSeparator12";
			resources->ApplyResources(this->toolStripSeparator12, L"toolStripSeparator12");
			// 
			// undoTools1
			// 
			this->undoTools1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->undoTools1, L"undoTools1");
			this->undoTools1->Name = L"undoTools1";
			// 
			// redoTools1
			// 
			this->redoTools1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->redoTools1, L"redoTools1");
			this->redoTools1->Name = L"redoTools1";
			// 
			// tools2
			// 
			resources->ApplyResources(this->tools2, L"tools2");
			this->tools2->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->MoveTools2, this->TerrainTools2, 
				this->PaintingTools2, this->EraserTools2, this->DeletingAddingTextureTools2, this->AreaIDTools2, this->toolStripSeparator10});
			this->tools2->Name = L"tools2";
			// 
			// MoveTools2
			// 
			this->MoveTools2->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->MoveTools2, L"MoveTools2");
			this->MoveTools2->Name = L"MoveTools2";
			// 
			// TerrainTools2
			// 
			this->TerrainTools2->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->TerrainTools2, L"TerrainTools2");
			this->TerrainTools2->Name = L"TerrainTools2";
			// 
			// PaintingTools2
			// 
			this->PaintingTools2->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->PaintingTools2, L"PaintingTools2");
			this->PaintingTools2->Name = L"PaintingTools2";
			// 
			// EraserTools2
			// 
			this->EraserTools2->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->EraserTools2, L"EraserTools2");
			this->EraserTools2->Name = L"EraserTools2";
			// 
			// DeletingAddingTextureTools2
			// 
			this->DeletingAddingTextureTools2->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->DeletingAddingTextureTools2, L"DeletingAddingTextureTools2");
			this->DeletingAddingTextureTools2->Name = L"DeletingAddingTextureTools2";
			// 
			// AreaIDTools2
			// 
			this->AreaIDTools2->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->AreaIDTools2, L"AreaIDTools2");
			this->AreaIDTools2->Name = L"AreaIDTools2";
			// 
			// toolStripSeparator10
			// 
			this->toolStripSeparator10->Name = L"toolStripSeparator10";
			resources->ApplyResources(this->toolStripSeparator10, L"toolStripSeparator10");
			// 
			// Form1
			// 
			resources->ApplyResources(this, L"$this");
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->tools2);
			this->Controls->Add(this->tools1);
			this->Controls->Add(this->menu);
			this->MainMenuStrip = this->menu;
			this->Name = L"Form1";
			this->menu->ResumeLayout(false);
			this->menu->PerformLayout();
			this->tools1->ResumeLayout(false);
			this->tools1->PerformLayout();
			this->tools2->ResumeLayout(false);
			this->tools2->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
};
}*/

