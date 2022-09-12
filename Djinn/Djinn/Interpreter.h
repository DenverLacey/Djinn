#pragma once

#include "BucketArray.h"
#include "Scope.h"

static constexpr size_t SCOPE_BUCKET_SIZE = 16;

struct Interpreter
{
	BucketArray<Scope> scopes = { SCOPE_BUCKET_SIZE };
};
