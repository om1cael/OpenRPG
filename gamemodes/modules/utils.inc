stock CreateInteractablePickupWithLabel(const label[], modelid, type, Float:x, Float:y, Float:z, labelDrawDistance)
{
    CreatePickup(modelid, type, x, y, z, 0);
    Create3DTextLabel(label, -1, x, y, z, labelDrawDistance, 0);
    return 1;
}