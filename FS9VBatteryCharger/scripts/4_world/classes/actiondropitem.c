modded class ActionDropItem : ActionSingleUseBase
{
	string m_ItemName = "";

	void ActionDropItem()
	{
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_DROPITEM_HANDS;
		m_CommandUIDProne	= DayZPlayerConstants.CMD_ACTIONFB_DROPITEM_HANDS;
		m_CallbackClass 	= ActionDropItemCB;
		m_Text 				= "#drop_item";
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem 	= new CCINone();
		m_ConditionTarget 	= new CCTNone();
	}
	
	override typename GetInputType()
	{
		return DropActionInput;
	}
	
	override bool HasProneException()
	{
		return true;
	}
	
	override bool CanBeUsedThrowing()
	{
		return true;
	}
	
	override bool HasTarget()
	{
		return false;
	}
	
	override bool UseAcknowledgment()
	{
		return false;
	}

	override bool ActionConditionContinue(ActionData action_data)
	{
		return true;
	}
	
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		HumanCommandMove hcm = player.GetCommand_Move();
		if (hcm && hcm.IsChangingStance())
			return false;
		
		if (player.GetCommand_Vehicle())
			return false;
		
		return player && item;
	}
	
	override void OnExecuteServer(ActionData action_data)
	{
		if (action_data.m_Player.IsPlacingServer())
			action_data.m_Player.PlacingCancelServer();

		if (!GetGame().IsMultiplayer()) 
			PhysicalDropItem(action_data);
		
		ItemBase targetItem = ItemBase.Cast(action_data.m_Player.GetItemInHands());
		if (targetItem.IsInherited(FSBatteryCharger))
		{
			FSBatteryCharger charger = FSBatteryCharger.Cast(targetItem);
			if (charger.GetCompEM().IsPlugged())
			{
				targetItem.SetObjectMaterial( 2, FSBatteryCharger.DEFAULT_MATERIAL_CABLE );
				charger.iscordedplugged = true;
				charger.SetSynchDirty();
			}
		}
	}
	
	override void OnExecuteClient(ActionData action_data)
	{
		super.OnExecuteClient(action_data);

		PhysicalDropItem(action_data);
	}
	
	override void PhysicalDropItem( ActionData action_data )
	{
		action_data.m_Player.PhysicalPredictiveDropItem(action_data.m_Player.GetItemInHands());
		
	}
}
