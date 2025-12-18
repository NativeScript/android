export function getHotData() {
  return import.meta?.hot?.data;
}

export function setHotValue(value) {
  const hot = import.meta?.hot;
  if (!hot || !hot.data) {
    throw new Error("import.meta.hot.data is not available");
  }
  hot.data.value = value;
  return hot.data.value;
}

export function getHotValue() {
  const hot = import.meta?.hot;
  return hot?.data?.value;
}

export function testHotApi() {
  const hot = import.meta?.hot;
  const result = {
    ok: false,
    hasHot: !!hot,
    hasData: !!hot?.data,
    hasAccept: typeof hot?.accept === "function",
    hasDispose: typeof hot?.dispose === "function",
    hasDecline: typeof hot?.decline === "function",
    hasInvalidate: typeof hot?.invalidate === "function",
    pruneIsFalse: hot?.prune === false,
  };

  try {
    hot?.accept?.(() => {});
    hot?.dispose?.(() => {});
    hot?.decline?.();
    hot?.invalidate?.();
    result.ok =
      result.hasHot &&
      result.hasData &&
      result.hasAccept &&
      result.hasDispose &&
      result.hasDecline &&
      result.hasInvalidate &&
      result.pruneIsFalse;
  } catch (e) {
    result.error = e?.message ?? String(e);
  }

  return result;
}
