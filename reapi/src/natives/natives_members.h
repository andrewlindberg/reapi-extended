#pragma once

void RegisterNatives_Members();

void *get_pdata_custom(CBaseEntity *pEntity, cell member);
cell set_member(void* pdata, const member_t *member, cell* value, size_t element);
cell get_member(void* pdata, const member_t *member, cell* dest, size_t element, size_t length = 0);

bool isTypeReturnable(MType type);
