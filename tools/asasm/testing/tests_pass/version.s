; Tests existence of {ASASM_VERSION} and |asasm$version| and that those
; are equal and reasonably sane.

	ASSERT {ASASM_VERSION} = |asasm$version|
	ASSERT {ASASM_VERSION} / 1000000 = 2

	END