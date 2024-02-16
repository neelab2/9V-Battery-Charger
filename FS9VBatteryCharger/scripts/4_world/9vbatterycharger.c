class FSBatteryCharger extends ItemBase
{

	// glow materials
	static protected const string RED_LIGHT_GLOW		= "dz\\gear\\camping\\data\\battery_charger_light_r.rvmat";
	static protected const string GREEN_LIGHT_GLOW		= "dz\\gear\\camping\\data\\battery_charger_light_g.rvmat";
	static protected const string YELLOW_LIGHT_GLOW		= "dz\\gear\\camping\\data\\battery_charger_light_y.rvmat";
	static protected const string SWITCH_LIGHT_GLOW		= "dz\\gear\\camping\\data\\battery_charger_light_switch_on.rvmat";
	static const string V_DEFAULT_MATERIAL 				= "FS9VBatteryCharger\\data\\FSBatteryCharger.rvmat";
	static const string DEFAULT_MATERIAL_CABLE 			= "dz\\gear\\camping\\data\\battery_charger.rvmat";
	static const string Inv_Materieal					= "";
	static protected const string Def_Materieal_Bat		= "dz\\gear\\consumables\\data\\9v.rvmat";
	

	
	
	int 						m_BatteryEnergyLeft0To100;
	int 						m_BatteryEnergyRight0To100;
	protected float 			m_ChargeEnergyPerSecond;
	
	static protected 	float 	m_BlinkingStatusLightInterval = 0.4; // How often the lights blink
	ref 				Timer 	m_UpdateStatusLightsTimer;
	protected 			bool 	m_BlinkingStatusLightLeftIsOn = false;	// Status of one blinking light
	protected 			bool 	m_BlinkingStatusLightIsOn = false;	// Status of one blinking light
	protected 			bool 	m_BlinkingStatusLightRightIsOn = false;	// Status of one blinking light
	
	bool iscordedplugged = false;
	
	void FSBatteryCharger()
	{
		m_ChargeEnergyPerSecond = GetGame().ConfigGetFloat ("CfgVehicles " + GetType() + " ChargeEnergyPerSecond");
		m_UpdateStatusLightsTimer = new Timer( CALL_CATEGORY_SYSTEM );
		SwitchLightOff();
		RegisterNetSyncVariableInt("m_BatteryEnergyLeft0To100");
		RegisterNetSyncVariableInt("m_BatteryEnergyRight0To100");
		RegisterNetSyncVariableBool("m_IsSoundSynchRemote");
		RegisterNetSyncVariableBool("m_IsPlaceSound");
		RegisterNetSyncVariableBool("iscordedplugged");

		SetObjectMaterial( 6, V_DEFAULT_MATERIAL );
		SetObjectMaterial( 8, V_DEFAULT_MATERIAL );
	}
	
	override bool IsElectricAppliance()
	{
		return true;
	}

	override void OnWork( float consumed_energy )
	{
		// Charging functionality

		int slotleft = InventorySlots.GetSlotIdFromString("BatteryD");
		int slotright = InventorySlots.GetSlotIdFromString("BatteryD2");
		ItemBase battery1 = ItemBase.Cast(GetInventory().FindAttachment(slotleft));
		ItemBase battery2 = ItemBase.Cast(GetInventory().FindAttachment(slotright));
		
		if ( battery1 )
		{
			if ( GetGame().IsServer() )
			{
				float battery_capacity = battery1.GetCompEM().GetEnergyMax();
				
				if ( battery1.GetCompEM().GetEnergy() < battery_capacity )
				{
					// Heat up the items so players know they are working.
					this.SetTemperature(60); 
					battery1.SetTemperature(60);
					
					float charger_health1 = GetHealth("", "");
					float energy_add1 = m_ChargeEnergyPerSecond * ( consumed_energy / GetCompEM().GetEnergyUsage() );
					
					#ifdef DIAG_DEVELOPER
					if (FeatureTimeAccel.GetFeatureTimeAccelEnabled(ETimeAccelCategories.ENERGY_RECHARGE))
					{
						float timeAccel = FeatureTimeAccel.GetFeatureTimeAccelValue();
						energy_add1 *= timeAccel;
					}
					#endif
					
					if ( GetCompEM().ConsumeEnergy(energy_add1) ) // consumes energy from the power source
					{
						// There is enough of energy to use
						energy_add1 = energy_add1 * ( 0.5 + charger_health1*0.005 ); // Damaged charger works less efficiently - 50% damage causes 75% efficiency
					}
					else
					{
						// There is NOT enough of energy to use
						energy_add1 = 0;
					}
					
					battery1.GetCompEM().AddEnergy( energy_add1 );
				}
				else
				{
					battery1.GetCompEM().SetEnergy( battery_capacity );
				}
				m_BatteryEnergyLeft0To100 = battery1.GetCompEM().GetEnergy0To100();
				SetSynchDirty();
			}
		}
		
		if ( battery2 )
		{
			if ( GetGame().IsServer() )
			{
				float battery_capacity2 = battery2.GetCompEM().GetEnergyMax();
				
				if ( battery2.GetCompEM().GetEnergy() < battery_capacity2 )
				{
					// Heat up the items so players know they are working.
					this.SetTemperature(60); 
					battery2.SetTemperature(60);
					
					float charger_health2 = GetHealth("", "");
					float energy_add2 = m_ChargeEnergyPerSecond * ( consumed_energy / GetCompEM().GetEnergyUsage() );
					
					#ifdef DIAG_DEVELOPER
					if (FeatureTimeAccel.GetFeatureTimeAccelEnabled(ETimeAccelCategories.ENERGY_RECHARGE))
					{
						float timeAccel = FeatureTimeAccel.GetFeatureTimeAccelValue();
						energy_add2 *= timeAccel;
					}
					#endif
					
					if ( GetCompEM().ConsumeEnergy(energy_add2) ) // consumes energy from the power source
					{
						// There is enough of energy to use
						energy_add2 = energy_add2 * ( 0.5 + charger_health2*0.005 ); // Damaged charger works less efficiently - 50% damage causes 75% efficiency
					}
					else
					{
						// There is NOT enough of energy to use
						energy_add2 = 0;
					}
					
					battery2.GetCompEM().AddEnergy( energy_add2 );
				}
				else
				{
					battery2.GetCompEM().SetEnergy( battery_capacity2 );
				}
				m_BatteryEnergyRight0To100 = battery2.GetCompEM().GetEnergy0To100();
				SetSynchDirty();
			}
		}
	}
	
	void SetIsPlugged(bool isplugged)
	{
		iscordedplugged = isplugged;
		SetSynchDirty();
	}

	override void OnWorkStart()
	{
		if ( GetGame().IsClient()  ||  !GetGame().IsMultiplayer() )
		{
			UpdateStatusLights();
			m_UpdateStatusLightsTimer.Run( m_BlinkingStatusLightInterval/2 , this, "UpdateStatusLights", NULL, true);
		}
	}
		
	override void OnWorkStop()
	{
		if ( GetGame().IsClient()  ||  !GetGame().IsMultiplayer() )
		{
			UpdateStatusLights();
			m_UpdateStatusLightsTimer.Stop();
		}
	}
	
	void UpdateStatusLights()
	{
		if ( GetGame().IsClient()  ||  !GetGame().IsMultiplayer() )
		{
			if (GetCompEM().IsWorking())
			{
				SwitchLightOn();

				
				int slotleft = InventorySlots.GetSlotIdFromString("BatteryD");
				int slotright = InventorySlots.GetSlotIdFromString("BatteryD2");
				ItemBase battery_left = ItemBase.Cast(GetInventory().FindAttachment(slotleft));
				ItemBase battery_right = ItemBase.Cast(GetInventory().FindAttachment(slotright));
				
				if (battery_left)
				{
					RedLightOff();
					
					if (m_BatteryEnergyLeft0To100 <= 33)
					{
						// Less than 1/3 charged, yellow status light must repeatedly blink
						
						if (m_BlinkingStatusLightLeftIsOn)
							YellowLightLEFTOn();
						else
							YellowLightLEFTOff();
						
						GreenLightLEFTOff();
						
						m_BlinkingStatusLightLeftIsOn = !m_BlinkingStatusLightLeftIsOn;
					}
					else if (m_BatteryEnergyLeft0To100 > 33  &&  m_BatteryEnergyLeft0To100 <= 66)
					{
						// Less than 2/3 charged, yellow status light must glow
						
						YellowLightLEFTOn();
						GreenLightLEFTOff();
					}
					else if (m_BatteryEnergyLeft0To100 > 66  &&  m_BatteryEnergyLeft0To100 < 100)
					{
						// Less than 3/3 charged, yellow status light must glow, green light must blink
						
						YellowLightLEFTOn();
						
						if (m_BlinkingStatusLightLeftIsOn)
							GreenLightLEFTOn();
						else
							GreenLightLEFTOff();
						
						m_BlinkingStatusLightLeftIsOn = !m_BlinkingStatusLightLeftIsOn;
					}
					else if (m_BatteryEnergyLeft0To100 >= 100)
					{
						// Fully charged, green light must glow
						YellowLightLEFTOff();
						GreenLightLEFTOn();
					}
				}
				else
				{
					GreenLightLEFTOff();
					YellowLightLEFTOff();
				}
				
				
				if (battery_right)
				{
					RedLightOff();
					
					if (m_BatteryEnergyRight0To100 <= 33)
					{
						// Less than 1/3 charged, yellow status light must repeatedly blink
						
						if (m_BlinkingStatusLightRightIsOn)
							YellowLightRIGHTOn();
						else
							YellowLightRIGHTOff();
						
						GreenLightRIGHTOff();
						
						m_BlinkingStatusLightRightIsOn = !m_BlinkingStatusLightRightIsOn;
					}
					else if (m_BatteryEnergyRight0To100 > 33  &&  m_BatteryEnergyRight0To100 <= 66)
					{
						// Less than 2/3 charged, yellow status light must glow
						
						YellowLightRIGHTOn();
						GreenLightRIGHTOff();
					}
					else if (m_BatteryEnergyRight0To100 > 66  &&  m_BatteryEnergyRight0To100 < 100)
					{
						// Less than 3/3 charged, yellow status light must glow, green light must blink
						
						YellowLightRIGHTOn();
						
						if (m_BlinkingStatusLightRightIsOn)
							GreenLightRIGHTOn();
						else
							GreenLightRIGHTOff();
						
						m_BlinkingStatusLightRightIsOn = !m_BlinkingStatusLightRightIsOn;
					}
					else if (m_BatteryEnergyRight0To100 >= 100)
					{
						// Fully charged, green light must glow
						YellowLightRIGHTOff();
						GreenLightRIGHTOn();
					}
				}
				else
				{
					GreenLightRIGHTOff();
					YellowLightRIGHTOff();
				}
				
				if (!battery_left && !battery_right)
				{
					if (m_BlinkingStatusLightIsOn)
					{
						RedLightOn();
					}
					else
					{
						RedLightOff();
					}
					m_BlinkingStatusLightIsOn = !m_BlinkingStatusLightIsOn;
					
				}
			}
			else
			{
				SwitchLightOff();
				GreenLightLEFTOff();
				YellowLightLEFTOff();
				GreenLightRIGHTOff();
				RedLightOff();
				YellowLightRIGHTOff();
			}
		}
	}
	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}
		// No "Take" action if the item is connected
		if ( !GetCompEM().IsPlugged()  &&  !GetCompEM().GetPluggedDevice() )
		{
			return true;
		}
		
		return false;
	}

	override bool CanPutIntoHands( EntityAI player ) 
	{
		if( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		// No "Take into hands" action if the item is connected
		if ( !GetCompEM().IsPlugged()  &&  !GetCompEM().GetPluggedDevice() )
		{
			return true;
		}
		
		return false;
	}

	override void OnOwnSocketTaken( EntityAI device )
	{
		string att_type = device.GetType();
		
		int slotleft = InventorySlots.GetSlotIdFromString("BatteryD");
		int slotright = InventorySlots.GetSlotIdFromString("BatteryD2");
		ItemBase battery_left = ItemBase.Cast(GetInventory().FindAttachment(slotleft));
		ItemBase battery_right = ItemBase.Cast(GetInventory().FindAttachment(slotright));
		
		if ( battery_left)
		{

			SetObjectMaterial( 3, Inv_Materieal );
			SetObjectMaterial( 3, Def_Materieal_Bat );

		}
		if (battery_right)
		{

			SetObjectMaterial( 4, Inv_Materieal );
			SetObjectMaterial( 4, Def_Materieal_Bat );

		}
		

	}

	override void OnOwnSocketReleased( EntityAI device )
	{
		int slotleft = InventorySlots.GetSlotIdFromString("BatteryD");
		int slotright = InventorySlots.GetSlotIdFromString("BatteryD2");
		ItemBase battery_left = ItemBase.Cast(GetInventory().FindAttachment(slotleft));
		ItemBase battery_right = ItemBase.Cast(GetInventory().FindAttachment(slotright));
		
		if (battery_left==NULL)
		{
			SetObjectMaterial( 3, Inv_Materieal );

		}
		if (battery_right==NULL)
		{
			SetObjectMaterial( 4, Inv_Materieal );

		}

	}

	override bool CanReceiveAttachment( EntityAI attachment, int slotId ) 
	{
		/*if ( !super.CanReceiveAttachment(attachment, slotId) )
			return false;
		
		ItemBase ibase;
		Class.CastTo(ibase, attachment);
		
		// No attaching if the charger is in inventory!
		PlayerBase charger_owner = PlayerBase.Cast( GetHierarchyRootPlayer() );
		if ( charger_owner )
			return false;
		
		// Only one attachment allowed
		if ( GetCompEM().GetPluggedDevice() )
			return false;
		
		if ( ibase.HasEnergyManager()  &&  ibase.GetCompEM().GetPluggedDevicesCount() >= 1 ) // Make sure nothing is plugged into the battery
			return false;
		*/
		// No attaching if the charger is in inventory!
		PlayerBase charger_owner = PlayerBase.Cast( GetHierarchyRootPlayer() );
		if ( charger_owner )
			return false;
		
		return true;
	}
	
	override bool CanLoadAttachment( EntityAI attachment) 
	{
		/*if ( !super.CanLoadAttachment(attachment) )
			return false;
		
		ItemBase ibase;
		Class.CastTo(ibase, attachment);
		
		// Only one attachment allowed
		if ( GetCompEM().GetPluggedDevice() )
			return false;
		
		if ( ibase.HasEnergyManager()  &&  ibase.GetCompEM().GetPluggedDevicesCount() >= 1 ) // Make sure nothing is plugged into the battery
			return false;
		*/
		
		return true;
	}
	
	
	
	// Control of status lights
	// ON
	void RedLightOn()
	{
		SetObjectMaterial( 9, RED_LIGHT_GLOW );
	}
	void GreenLightLEFTOn()
	{
		SetObjectMaterial( 6, GREEN_LIGHT_GLOW );
	}
	void YellowLightLEFTOn()
	{
		SetObjectMaterial( 5, YELLOW_LIGHT_GLOW );
	}
	void GreenLightRIGHTOn()
	{
		SetObjectMaterial( 8, GREEN_LIGHT_GLOW );
	}
	void YellowLightRIGHTOn()
	{
		SetObjectMaterial( 7, YELLOW_LIGHT_GLOW );
	}
	void SwitchLightOn()
	{
		SetObjectMaterial( 10, Inv_Materieal );
		SetObjectMaterial( 11, SWITCH_LIGHT_GLOW );
	}
	// OFF
	void RedLightOff()
	{
		SetObjectMaterial( 9, V_DEFAULT_MATERIAL );
	}
	void GreenLightLEFTOff()
	{
		SetObjectMaterial( 6, V_DEFAULT_MATERIAL );
	}
	void YellowLightLEFTOff()
	{
		SetObjectMaterial( 5, V_DEFAULT_MATERIAL );
	}
	void GreenLightRIGHTOff()
	{
		SetObjectMaterial( 8, V_DEFAULT_MATERIAL );
	}
	void YellowLightRIGHTOff()
	{
		SetObjectMaterial( 7, V_DEFAULT_MATERIAL );
	}
	void SwitchLightOff()
	{
		SetObjectMaterial( 10, V_DEFAULT_MATERIAL );
		SetObjectMaterial( 11, Inv_Materieal );
	}
	
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if ( !super.OnStoreLoad( ctx, version ) ) {
			return false;
		}
		if (!ctx.Read( iscordedplugged )) {
			return false;
		}
		return true;
	}
	
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
		ctx.Write( iscordedplugged );

	}
	
	override void AfterStoreLoad()
	{    
		super.AfterStoreLoad();
		
		if (iscordedplugged)
		{
			SetObjectMaterial( 2, V_DEFAULT_MATERIAL );
		}
		else
		{
			SetObjectMaterial( 2, Inv_Materieal );
		}
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
				
		if ( IsPlaceSound() )
		{
			PlayPlaceSound();
		}
		
		if (iscordedplugged)
		{
			SetObjectMaterial( 2, V_DEFAULT_MATERIAL );
		}
		else
		{
			SetObjectMaterial( 2, Inv_Materieal );
		}
	}
	
		
	//================================================================
	// ADVANCED PLACEMENT
	//================================================================
	
	
	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" )
	{
		super.OnPlacementComplete( player, position, orientation );
		if (GetCompEM().IsPlugged())
		{
			SetObjectMaterial( 2, DEFAULT_MATERIAL_CABLE );
			iscordedplugged = true;
			SetSynchDirty();
		}		
		SetIsPlaceSound( true );
	}
	
	override bool IsDeployable()
	{
		return true;
	}
	
	override string GetPlaceSoundset()
	{
		return "placeBatteryCharger_SoundSet";
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionPlugIn);
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionUnplugThisByCord);
		AddAction(ActionTurnOnWhileOnGround);		
		AddAction(ActionTurnOffWhileOnGround);
		AddAction(ActionPlaceObject);
	}
}