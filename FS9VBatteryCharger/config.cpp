class CfgPatches
{
	class FS9VBatteryCharger
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Scripts",
			"DZ_Gear_Camping",
			"DZ_Gear_Consumables"
		};
	};
};
class CfgMods
{
	class FS9VBatteryCharger
	{
		dir="FS9VBatteryCharger";
		name="FS9VBatteryCharger";
		picture="";
		action="";
		extra=0;
		hideName=1;
		hidePicture=1;
		credits="";
		version="0";
		type="mod";
		dependencies[]=
		{
			"World"
		};
		class defs
		{
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"FS9VBatteryCharger/scripts/4_world"
				};
			};
		};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class FSBatteryCharger: Inventory_Base
	{
		scope=2;
		displayName="9V Batterieladegerät";
		descriptionShort="Schnellladegerät für 9V Batterien";
		model="\FS9VBatteryCharger\9vbatterycharger.p3d";
		slopeTolerance=0.15000001;
		yawPitchRollLimit[]={45,45,45};
		attachments[]=
		{
			"BatteryD",
			"BatteryD2"
		};
		weight=50;
		itemSize[]={1,1};
		itemBehaviour=1;
		rotationFlags=2;
		hiddenSelections[]=
		{
			"schild",
			"base",
			"9vcord_plugged",
			"clips_batteryleft",
			"clips_batteryright",
			"light_charging_left",
			"light_charged_left",
			"light_charging_right",
			"light_charged_right",
			"9vlight_stand_by",
			"9vswitch_off",
			"9vswitch_on"
		};
		hiddenSelectionsTextures[]=
		{
			"FS9VBatteryCharger\data\schild_co.paa",
			"FS9VBatteryCharger\data\rusty_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"FS9VBatteryCharger\data\schild.rvmat",
			"FS9VBatteryCharger\data\FSBatteryCharger.rvmat"
		};
		hologramMaterial="batterycharger";
		hologramMaterialPath="FS9VBatteryCharger";
		ChargeEnergyPerSecond=0.2;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=90;
					transferToAttachmentsCoef=0.5;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"FS9VBatteryCharger\data\FSBatteryCharger.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"FS9VBatteryCharger\data\FSBatteryCharger.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"FS9VBatteryCharger\data\FSBatteryCharger_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"FS9VBatteryCharger\data\FSBatteryCharger_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"FS9VBatteryCharger\data\FSBatteryCharger_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		repairableWithKits[]={7};
		repairCosts[]={25};
		soundImpactType="metal";
		class EnergyManager
		{
			hasIcon=1;
			energyUsagePerSecond=0.0099999998;
			cordTextureFile="DZ\gear\camping\Data\plug_black_CO.paa";
			cordLength=5;
			plugType=2;
			compatiblePlugTypes[]={4};
			powerSocketsCount=2;
			attachmentAction=2;
			wetnessExposure=0.1;
		};
		class AnimationSources
		{
			class 9vcord_plugged
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class 9vswitch_on
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class 9vswitch_off
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class clips_batteryleft
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class clips_batteryright
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class light_charging_left
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class light_charged_left
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class light_charging_right
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class light_charged_right
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class 9vlight_stand_by
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class drop
				{
					soundset="batterycharger_drop_SoundSet";
					id=898;
				};
			};
		};
	};
	class Battery9V: Inventory_Base
	{
		scope=2;
		displayName="$STR_CfgVehicles_Battery9V0";
		descriptionShort="$STR_CfgVehicles_Battery9V1";
		model="\dz\gear\consumables\9v_battery.p3d";
		isMeleeWeapon=1;
		weight=10;
		weightPerQuantityUnit=0;
		absorbency = 0.1;
		itemSize[]={1,1};
		stackedUnit="w";
		quantityBar=1;
		varQuantityInit = 50;
		varQuantityMin = 0;
		varQuantityMax = 50;
		varQuantityDestroyOnMin=0;
		tooltipResources[] = {"power"};
		inventorySlot[]=
		{
			"BatteryD",
			"BatteryD2"
		};
		rotationFlags=17;
		class EnergyManager
		{
			hasIcon = 1;
			switchOnAtSpawn = 1;
			isPassiveDevice = 1;
			energyStorageMax = 50;
			energyAtSpawn = 50;
			reduceMaxEnergyByDamageCoef = 0;
			powerSocketsCount = 1;
			plugType = 4;
			compatiblePlugTypes[] = {1};
			attachmentAction = 2;
			convertEnergyToQuantity = 1;
		};
		class AnimationSources
		{
			class cover
			{
				source="user";
				animPeriod=0.5;
				initPhase=1;
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=20;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\gear\consumables\data\9v.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\gear\consumables\data\9v.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\gear\consumables\data\9v_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\gear\consumables\data\9v_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\gear\consumables\data\9v_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
};
class CfgSlots
{
	class Slot_BatteryD2
	{
		name="BatteryD2";
		displayName="#STR_CfgBatteryD0";
		ghostIcon="set:dayz_inventory image:batteryd";
	};
};