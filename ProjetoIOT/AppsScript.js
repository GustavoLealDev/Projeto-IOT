function doGet(e) {
  // Verifica se os parâmetros esperados foram recebidos
  if (!e.parameter.nome || !e.parameter.data || !e.parameter.hora || !e.parameter.tipo) {
    return ContentService.createTextOutput("Parâmetros incompletos.");
  }

  // Abre a planilha pelo ID ou pela aba ativa (se for o único script)
  const sheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName("Registro");
  
  if (!sheet) {
    return ContentService.createTextOutput("A aba 'Registros' não foi encontrada.");
  }

  // Coleta os dados dos parâmetros
  const nome = e.parameter.nome;
  const data = e.parameter.data;
  const hora = e.parameter.hora;
  const tipo = e.parameter.tipo;

  // Decide em qual coluna gravar ENTRADA ou SAÍDA
  let ultimaLinha = sheet.getLastRow() + 1;

  // Adiciona nova linha com os dados
  sheet.getRange(ultimaLinha, 1).setValue(nome);
  sheet.getRange(ultimaLinha, 2).setValue(data);
  if (tipo === "ENTRADA") {
    sheet.getRange(ultimaLinha, 3).setValue(hora);
  } else {
    sheet.getRange(ultimaLinha, 4).setValue(hora);
  }

  return ContentService.createTextOutput("Dados recebidos com sucesso.");
}
